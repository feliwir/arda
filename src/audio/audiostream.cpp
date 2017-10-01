#include "audiostream.hpp"
#include "audiobuffer.hpp"
#include "audio.hpp"
#include "../filesystem/avstream.hpp"
#include "../core/debugger.hpp"
#include "../core/exception.hpp"
#include <chrono>
#include <array>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/avutil.h>
}

namespace arda
{
	class AudioStreamInternals
	{
	public:
		static const int chainsize = 5;
		std::unique_ptr<AvStream> avstream;
		AVCodec* codec = nullptr;
		AVFrame* audioFrame = nullptr;
		AVFrame* resampledFrame = nullptr;
		AVCodecContext* codec_ctx = nullptr;
		AVFormatContext* format_ctx = nullptr;
		AVSampleFormat out_fmt;
		AVPacket* packet;
		ALuint source = 0;
		SwrContext* resampler = nullptr;
		bool needsResample = false;
		uint8_t** resampleData = nullptr;
		int resampleLinesize = 0;
		std::array<std::shared_ptr<AudioBuffer>, chainsize> bufferchain;
		int cbuffer = 0;
		std::chrono::high_resolution_clock::time_point last;
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
	auto& bufferchain = m_internals->bufferchain;
	auto& source = m_internals->source;
	auto& resampler = m_internals->resampler;
	auto& needsResample = m_internals->needsResample;
	auto& out_fmt = m_internals->out_fmt;
	auto& resampleData = m_internals->resampleData;
	auto& resampleLinesize = m_internals->resampleLinesize;
	auto& resampledFrame = m_internals->resampledFrame;
	auto& packet = m_internals->packet;

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

	//setup sampler and set OpenAL input format
	uint64_t in_layout;

	switch (sample_fmt)
	{
	case AV_SAMPLE_FMT_S16P:
		in_layout = codec_ctx->channel_layout;
		out_fmt = AV_SAMPLE_FMT_S16;
		needsResample = true;
	case AV_SAMPLE_FMT_S16:
		if (m_channels == 2)
		{
			fmt = AL_FORMAT_STEREO16;
		}
		else
		{
			fmt = AL_FORMAT_MONO16;
		}
		break;
	case AV_SAMPLE_FMT_U8P:
		in_layout = codec_ctx->channel_layout;
		out_fmt = AV_SAMPLE_FMT_U8;
		needsResample = true;
	case AV_SAMPLE_FMT_U8:
		if (m_channels == 2)
		{
			fmt = AL_FORMAT_STEREO8;
		}
		else
		{
			fmt = AL_FORMAT_MONO8;
		}
		break;
	default:
		in_layout = codec_ctx->channel_layout;
		fmt = AL_FORMAT_STEREO16;
		out_fmt = AV_SAMPLE_FMT_S16;
		needsResample = true;
		break;
	}

	//setup our resampler
	if (needsResample)
	{
		resampledFrame = av_frame_alloc();
		resampledFrame->channel_layout = in_layout;
		resampledFrame->sample_rate = m_frequency;
		resampledFrame->format = out_fmt;

		resampler = swr_alloc_set_opts(NULL,
			in_layout,		//Out layout should be identical to input layout
			out_fmt,
			m_frequency,	//Out frequency should be identical to input frequency
			in_layout,
			sample_fmt,
			m_frequency,
			0, NULL);		//No logging

		if (swr_init(resampler) != 0)
			throw RuntimeException("Could not init resampler!");
	}

	for (int i=0;i<bufferchain.size();++i)
	{
		bufferchain[i] = std::make_shared<AudioBuffer>(m_frequency, fmt);
	}


	alGenSources(1,&source);
	Audio::checkErrorAl("Failed to create AL source!");

	packet = av_packet_alloc();
	
	for(int i=0;i<AudioStreamInternals::chainsize;++i)
		UpdateBuffers();
}

arda::AudioStream::~AudioStream()
{	
	m_mutex.lock();
	m_state = STOPPED;
	auto& source = m_internals->source;

	// //make sure all buffers are unqueued:
	// int processed = 0;
	// alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
	// ALuint freed = 0;
	// alSourceUnqueueBuffers(source, processed, &freed);
	// Audio::checkErrorAl("Cannot unqueue the buffers");

	alDeleteSources(1, &source);
	Audio::checkErrorAl("Cannot delete the source");
	source = 0;

	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec = m_internals->codec;

	avcodec_close(codec_ctx);
	avformat_close_input(&format_ctx);
	m_mutex.unlock();
}

void arda::AudioStream::Start()
{
	//get the thread pool
	auto& pool = GetGlobal().GetThreadPool();
	auto& source = m_internals->source;
	auto& last = m_internals->last;

	m_state = PLAYING;

	pool.AddJob([this,&source,&last]()
	{

		while(m_state == PLAYING)
		{
			m_mutex.lock();
			//get all processed buffers:
			int processed = 0;
			alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
			ALuint freed[AudioStreamInternals::chainsize];
			alSourceUnqueueBuffers(source, processed, freed);
			Audio::checkErrorAl("Cannot unqueue the buffers");

			//check if until we actually need new buffers
			int queued = 0;
			alGetSourcei(source,AL_BUFFERS_QUEUED,&queued);
			Audio::checkErrorAl("Cannot get queued buffers");

			//q until we are back to 3
			for (; queued < AudioStreamInternals::chainsize; ++queued)
			{
				if (!UpdateBuffers())
				{
					m_state = STOPPED;
					break;
				}
			}
			m_mutex.unlock();

			m_position = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last).count()/1000.0;

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	});

	alSourcePlay(source);
	last = std::chrono::high_resolution_clock::now();
	Audio::checkErrorAl("Can't start the source playback");

	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	if(state!= AL_PLAYING)
		Audio::checkErrorAl("Source didn't start playing");

}

void arda::AudioStream::Pause()
{
	auto& source = m_internals->source;
	m_state = PAUSED;
	alSourcePause(source);
}

void arda::AudioStream::Stop()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto& source = m_internals->source;
	m_state = STOPPED;
	alSourceStop(source);
}

double arda::AudioStream::GetPosition()
{
	return m_position;
}

void arda::AudioStream::SetVolume(int volume)
{
	auto& source = m_internals->source;
	float gain = volume / 100.0;
	alSourcef(source, AL_GAIN, gain);
}

bool arda::AudioStream::UpdateBuffers()
{
	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec = m_internals->codec;
	auto& frame = m_internals->audioFrame;
	auto& bufferchain = m_internals->bufferchain;
	auto& source = m_internals->source;
	auto& needsResample = m_internals->needsResample;
	auto& resampler = m_internals->resampler;
	auto& out_fmt = m_internals->out_fmt;
	auto& cbuffer = m_internals->cbuffer;
	auto& packet = m_internals->packet;
	auto& resampledFrame = m_internals->resampledFrame;

	bool frameFinished = false;
	bool updatedColor = false, updatedAlpha = false;

	while (av_read_frame(format_ctx, packet) >= 0)
	{
		// Decode audio frame
		int res = avcodec_send_packet(codec_ctx,packet);
		if (avcodec_receive_frame(codec_ctx, frame) == 0)
			frameFinished = true;

		//int consumed = avcodec_decode_audio4(codec_ctx, frame, &frameFinished, &packet);
		double pts = av_frame_get_best_effort_timestamp(frame);
		pts *= av_q2d(codec_ctx->time_base);
		//can't use this pts as position since we are prebuffering :-/

		int data_size = av_samples_get_buffer_size(NULL, codec_ctx->channels,frame->nb_samples, codec_ctx->sample_fmt, 1);

		if (frameFinished)
		{
			uint8_t* data;
			int linesize = 0;

			m_position = packet->pts / static_cast<double>(AV_TIME_BASE);

			//check if we must resample:
			if (needsResample)
			{				
				swr_convert_frame(resampler, resampledFrame, frame);
				data_size = av_samples_get_buffer_size(&linesize, m_channels, frame->nb_samples, out_fmt, 0);
				data = resampledFrame->data[0];
			}
			else
			{
				data = frame->data[0];
			}

			auto& buffer = bufferchain[cbuffer];
			ALuint handle = buffer->GetHandle();


			//got a finished frame here
			buffer->Upload(data, data_size);

			//investigate more
			if (needsResample)
			{
				//Crash here, data is a valid pointer at that time
				//av_freep(&data);
			}

			alSourceQueueBuffers(source, 1, &handle);
			Audio::checkErrorAl("Failed to query buffer to source");

			++cbuffer;
			cbuffer %= AudioStreamInternals::chainsize;
			return true;
		}

	}

	return false;
}
