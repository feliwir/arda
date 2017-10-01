#include "video.hpp"
#include "../filesystem/avstream.hpp"
#include "../core/exception.hpp"
#include "../core/debugger.hpp"
#include <chrono>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

namespace arda
{
	class VideoInternals
	{
	public:
		std::unique_ptr<AvStream> avstream;
		AVCodec* codec;
		AVCodec* codec_a;
		AVCodecContext* codec_ctx;
		AVCodecContext* codec_ctx_a;
		AVFormatContext* format_ctx;
		AVFrame* rgbFrame;
		AVFrame* alphaFrame;
		uint8_t* rgbBuffer;
		uint8_t* alphaBuffer;
		AVFrame* tmpFrame;
		SwsContext* swsContext;
		AVPacket* packet;

		int decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt)
		{
			int ret;

			*got_frame = 0;
			
			if (pkt) {
				ret = avcodec_send_packet(avctx, pkt);
				// In particular, we don't expect AVERROR(EAGAIN), because we read all
				// decoded frames with avcodec_receive_frame() until done.
				if (ret < 0)
					return ret == AVERROR_EOF ? 0 : ret;
			}

			ret = avcodec_receive_frame(avctx, frame);
			if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
				return ret;
			if (ret >= 0)
				*got_frame = 1;

			return 0;
		}

	};
}

arda::Video::Video(std::shared_ptr<IStream> stream) :
	m_internals(std::make_unique<VideoInternals>()),
	m_hasAlpha(false),
	m_width(0),
	m_height(0),
	m_curFrame(0),
	m_fps(0.0),
	m_position(0.0),
	m_frameTime(0)
{
	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec_ctx_a = m_internals->codec_ctx_a;
	auto& codec = m_internals->codec;
	auto& codec_a = m_internals->codec_a;
	auto& tmpFrame = m_internals->tmpFrame;
	auto& rgbFrame = m_internals->rgbFrame;
	auto& alphaFrame = m_internals->alphaFrame;
	auto& rgbBuffer = m_internals->rgbBuffer;
	auto& alphaBuffer = m_internals->alphaBuffer;
	auto& swsContext = m_internals->swsContext;
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
	
	int vid_streams = 0;
	for (int i = 0; i < format_ctx->nb_streams; i++)
	{
		if (format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			vid_streams++;
			AVRational fps = format_ctx->streams[i]->avg_frame_rate;
			m_fps = fps.num / fps.den;
			m_numFrames = format_ctx->streams[i]->nb_frames;
			m_frameTime = (1.0 / m_fps) * 1000000;
		}			
	}

	if(vid_streams<1)
		throw RuntimeException("No video stream contained!");

	auto origCtx = format_ctx->streams[COLOR]->codec;

	m_rgbImage.Create(glm::ivec2(origCtx->width, origCtx->height),
								 gli::format::FORMAT_RGB8_UNORM_PACK8, nullptr);

	if (vid_streams > 1)
	{
		m_hasAlpha = true;
		auto origCtxA = format_ctx->streams[ALPHA]->codec;

		// Find the decoder for the video stream
		codec_a = avcodec_find_decoder(origCtxA->codec_id);
		if (codec_a == NULL)
			throw RuntimeException("Unsupported codec!");

		// Copy context
		codec_ctx_a = avcodec_alloc_context3(codec_a);
		if (avcodec_copy_context(codec_ctx_a, origCtxA) != 0)
		{
			fprintf(stderr, "Couldn't copy codec context");
			throw RuntimeException("Couldn't copy codec ctx!");
		}

		// Open codec
		if (avcodec_open2(codec_ctx_a, codec_a, NULL)<0)
			throw RuntimeException("Could not open codec!");

		m_alphaImage.Create(glm::ivec2(codec_ctx_a->width, codec_ctx_a->height),
							gli::format::FORMAT_RGB8_UNORM_PACK8, nullptr);
	}
		
	// Find the decoder for the video stream
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
	if (avcodec_open2(codec_ctx, codec,NULL)<0)
		throw RuntimeException("Could not open codec!");
	
	rgbFrame =  av_frame_alloc();
	tmpFrame = av_frame_alloc();
	alphaFrame = av_frame_alloc();

	int numBytes;

	m_width = codec_ctx->width;
	m_height = codec_ctx->height;

	// Determine required buffer size and allocate buffer
	numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, codec_ctx->width,
		codec_ctx->height);

	rgbBuffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
	avpicture_fill((AVPicture *)rgbFrame, rgbBuffer, AV_PIX_FMT_RGB24,
		codec_ctx->width, codec_ctx->height);

	alphaBuffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
	avpicture_fill((AVPicture *)alphaFrame, alphaBuffer, AV_PIX_FMT_RGB24,
		codec_ctx->width, codec_ctx->height);

	// initialize SWS context for software scaling
	swsContext = sws_getContext(codec_ctx->width,
		codec_ctx->height,
		codec_ctx->pix_fmt,
		codec_ctx->width,
		codec_ctx->height,
		AV_PIX_FMT_RGB24,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);

	avcodec_close(origCtx);
	
	packet = av_packet_alloc();
	av_init_packet(packet);

	//read the first 2 frames already
	GetFrames();
	if (m_hasAlpha)
		GetFrames();
}

arda::Video::~Video()
{
	m_state = STOPPED;
	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec_ctx_a = m_internals->codec_ctx_a;
	auto& codec = m_internals->codec;
	auto& tmpFrame = m_internals->tmpFrame;
	auto& rgbFrame = m_internals->rgbFrame;
	auto& alphaFrame = m_internals->alphaFrame;
	auto& rgbBuffer = m_internals->rgbBuffer;
	auto& alphaBuffer = m_internals->alphaBuffer;
	auto& packet = m_internals->packet;

	av_free(alphaBuffer);
	av_free(rgbBuffer);
	av_free(tmpFrame);
	av_free(rgbFrame);

	av_packet_free(&packet);

	avcodec_close(codec_ctx);
	avcodec_close(codec_ctx_a);
	avformat_close_input(&format_ctx);
}

void arda::Video::Start()
{
	//get the thread pool
	auto& pool = GetGlobal().GetThreadPool();

	m_state = PLAYING;

	pool.AddJob([this]()
	{
		auto last = std::chrono::high_resolution_clock::now();
		while (m_state == PLAYING)
		{
			if (!GetFrames())
			{
				m_state = STOPPED;
				break;
			}
			std::this_thread::sleep_until(last+std::chrono::microseconds(m_frameTime));
			last = last + std::chrono::microseconds(m_frameTime);
		}

	});
}

void arda::Video::Pause()
{
	m_state = PAUSED;
}

void arda::Video::Stop()
{
	m_state = STOPPED;
}

double arda::Video::GetPosition()
{
	return m_position;
}


bool arda::Video::GetFrames()
{
	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec_ctx_a = m_internals->codec_ctx_a;
	auto& codec = m_internals->codec;
	auto& tmpFrame = m_internals->tmpFrame;
	auto& rgbFrame = m_internals->rgbFrame;
	auto& alphaFrame = m_internals->alphaFrame;
	auto& rgbBuffer = m_internals->rgbBuffer;
	auto& swsContext = m_internals->swsContext;
	auto& packet = m_internals->packet;

	int gotFrame =0;
	bool updatedColor = false, updatedAlpha = false;
	bool keepGoing = false;

	if (packet == nullptr)
		throw RuntimeException("Packet is null!");

	while (av_read_frame(format_ctx, packet) >= 0)
	{
		if (packet == NULL)
		{
			ARDA_LOG("Empty packet");
			continue;
		}
			

		keepGoing = true;
		AVCodecContext* codec = (packet->stream_index==COLOR) ?
								codec_ctx : codec_ctx_a;
		AVFrame* tgtFrame = (packet->stream_index == COLOR) ?
								rgbFrame : alphaFrame;
		Image& tgtImage = (packet->stream_index == COLOR) ?
							m_rgbImage : m_alphaImage;


		// Decode video frame
		m_internals->decode(codec, tmpFrame, &gotFrame, packet);

		double pts = av_frame_get_best_effort_timestamp(tmpFrame);
		pts *= av_q2d(codec->time_base);
		m_position = pts;

		if (gotFrame)
		{
			// Convert the image from its native format to RGB
			sws_scale(swsContext, (uint8_t const * const *)tmpFrame->data,
				tmpFrame->linesize, 0, codec->height,
				tgtFrame->data, tgtFrame->linesize);

			tgtImage.Update(tgtFrame->data[0]);

			if (packet->stream_index == COLOR)
				updatedColor = true;			
			else if (packet->stream_index == ALPHA)
				updatedAlpha = true;
		}


		if (updatedColor && (updatedAlpha || !m_hasAlpha))
			break;
	}

	if (!keepGoing)
	{
		int a = 0;
	}

	return keepGoing;
}
