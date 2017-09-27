#include "audiostream.hpp"
#include "audiobuffer.hpp"
#include "../filesystem/avstream.hpp"
#include "../core/exception.hpp"
#include <chrono>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

namespace arda
{
	class AudioStreamInternals
	{
	public:
		std::unique_ptr<AvStream> avstream;
		AVCodec* codec;
		AVFrame* audioFrame;
		AVCodecContext* codec_ctx;
		AVFormatContext* format_ctx;
		std::unique_ptr<AudioBuffer> frontBuffer;
		std::unique_ptr<AudioBuffer> backBuffer;
	};
}

arda::AudioStream::AudioStream(std::shared_ptr<IStream> stream) :
	m_internals(std::make_unique<AudioStreamInternals>()),
	m_bitrate(0),
	m_position(0.0)
{
	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec = m_internals->codec;
	auto& frame = m_internals->audioFrame;
	auto& fbuffer = m_internals->frontBuffer;
	auto& bbuffer = m_internals->backBuffer;

	avstream = std::make_unique<AvStream>(stream);
	format_ctx = avformat_alloc_context();
	avstream->Attach(m_internals->format_ctx);

	if (avformat_open_input(&format_ctx, "", 0, 0) != 0)
		throw RuntimeException("Failed to open video file!");

	// Retrieve stream information
	if (avformat_find_stream_info(format_ctx, NULL)<0)
		throw RuntimeException("Failed retrieve stream information!");

	m_duration = format_ctx->duration / static_cast<double>(AV_TIME_BASE);

	if(format_ctx->nb_streams!=1 || !(format_ctx->streams[0]->codec->codec_type == AVMEDIA_TYPE_AUDIO))
		throw RuntimeException("An audio file must contain exactly 1 audio stream!");

	auto origCtx = format_ctx->streams[0]->codec;

	// Find the decoder for the audio stream
	codec = avcodec_find_decoder(origCtx->codec_id);
	if (codec == NULL)
		throw RuntimeException("Unsupported codec!");

	// Copy context
	codec_ctx = avcodec_alloc_context3(codec);
	if (avcodec_copy_context(codec_ctx, origCtx) != 0) 
	{
		fprintf(stderr, "Couldn't copy codec context");
		throw RuntimeException("Couldn't copy codec ctx!");
	}

	// Open codec
	if (avcodec_open2(codec_ctx, codec, NULL)<0)
		throw RuntimeException("Could not open codec!");

	frame = av_frame_alloc();

	m_bitrate = format_ctx->bit_rate;
	m_frequency = codec_ctx->sample_rate;
	m_channels = codec_ctx->channels;

	//create our buffers
	AVSampleFormat sample_fmt = codec_ctx->sample_fmt;

	ALenum fmt = AL_FORMAT_MONO8;
		
	switch (sample_fmt)
	{
	case AV_SAMPLE_FMT_S16P:
	case AV_SAMPLE_FMT_S16:
		(m_channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		break;
	case AV_SAMPLE_FMT_U8:
	case AV_SAMPLE_FMT_U8P:
		(m_channels == 2) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		break;
	}

	fbuffer = std::make_unique<AudioBuffer>(m_frequency, fmt);
	bbuffer = std::make_unique<AudioBuffer>(m_frequency, fmt);
}

arda::AudioStream::~AudioStream()
{
	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec = m_internals->codec;

	avcodec_close(codec_ctx);
	avformat_close_input(&format_ctx);
}

void arda::AudioStream::Start()
{
	//get the thread pool
	auto& pool = GetGlobal().GetThreadPool();

	m_state = PLAYING;

	pool.AddJob([this]()
	{
		auto last = std::chrono::high_resolution_clock::now();
		while (m_state == PLAYING)
		{
			UpdateBuffers();
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}

	});
}

void arda::AudioStream::Pause()
{
}

void arda::AudioStream::Stop()
{
}

double arda::AudioStream::GetPosition()
{
	return 0.0;
}

void arda::AudioStream::UpdateBuffers()
{
	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec = m_internals->codec;
	auto& frame = m_internals->audioFrame;
	auto& fbuffer = m_internals->frontBuffer;
	auto& bbuffer = m_internals->backBuffer;

	int frameFinished;
	AVPacket packet;
	bool updatedColor = false, updatedAlpha = false;

	while (av_read_frame(format_ctx, &packet) >= 0)
	{
		// Decode video frame
		avcodec_decode_audio4(codec_ctx, frame, &frameFinished, &packet);
		double pts = packet.pts;
		double duration = frame->pkt_duration / static_cast<double>(AV_TIME_BASE);
		
		int data_size = av_samples_get_buffer_size(NULL, codec_ctx->channels,frame->nb_samples, codec_ctx->sample_fmt, 1);

		if (frameFinished)
		{
			fbuffer->Upload(frame->data[0], data_size);
			bbuffer->Upload(frame->data[1], data_size);
		}
	}
}
