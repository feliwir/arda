#include "video.hpp"
#include "../filesystem/avstream.hpp"
#include "../core/exception.hpp"
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
		AVCodecContext* codec_ctx;
		AVCodecContext* codec_ctx_a;
		AVFormatContext* format_ctx;
		AVFrame* m_rgbFrame;
		AVFrame* m_alphaFrame;
		AVFrame* m_tmpFrame;
		uint8_t* m_rgbBuffer;
	};
}

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
	FILE *pFile;
	char szFilename[32];
	int  y;

	// Open file
	sprintf(szFilename, "frame%d.ppm", iFrame);
	pFile = fopen(szFilename, "wb");
	if (pFile == NULL)
		return;

	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	// Write pixel data
	for (y = 0; y<height; y++)
		fwrite(pFrame->data[0] + y*pFrame->linesize[0], 1, width * 3, pFile);

	// Close file
	fclose(pFile);
}

arda::Video::Video(std::shared_ptr<IStream> stream) :
	m_internals(std::make_unique<VideoInternals>()),
	m_hasAlpha(false)
{
	av_register_all();
	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec_ctx_a = m_internals->codec_ctx_a;
	auto& codec = m_internals->codec;
	auto& tmpFrame = m_internals->m_tmpFrame;
	auto& rgbFrame = m_internals->m_rgbFrame;
	auto& alphaFrame = m_internals->m_alphaFrame;
	auto& rgbBuffer = m_internals->m_rgbBuffer;


	avstream = std::make_unique<AvStream>(stream);
	format_ctx = avformat_alloc_context();
	avstream->Attach(m_internals->format_ctx);

	if (avformat_open_input(&format_ctx, "", 0, 0) != 0)
		throw RuntimeException("Failed to open video file!");

	// Retrieve stream information
	if (avformat_find_stream_info(format_ctx, NULL)<0)
		throw RuntimeException("Failed retrieve stream information!");

	// Dump information about file onto standard error
	//av_dump_format(m_internals->format_ctx,  0, "", 0);

	int vid_streams = 0;
	for (int i = 0; i < format_ctx->nb_streams; i++)
	{
		if (format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			vid_streams++;
	}
	
	if(vid_streams<1)
		throw RuntimeException("No video stream contained!");

	auto origCtx = format_ctx->streams[0]->codec;

	if (vid_streams > 1)
	{
		m_hasAlpha = true;
		codec_ctx_a = format_ctx->streams[1]->codec;
	}
		
	// Find the decoder for the video stream
	codec = avcodec_find_decoder(origCtx->codec_id);
	if (codec == NULL)
		throw RuntimeException("Unsupported codec!");

	// Copy context
	codec_ctx = avcodec_alloc_context3(codec);
	if (avcodec_copy_context(codec_ctx, origCtx) != 0) {
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

	struct SwsContext *sws_ctx = NULL;
	int frameFinished;
	AVPacket packet;

	// initialize SWS context for software scaling
	sws_ctx = sws_getContext(codec_ctx->width,
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

	int i = 0;
	while (av_read_frame(format_ctx, &packet) >= 0) {
		// Is this a packet from the video stream?
		if (packet.stream_index == 0) {
			// Decode video frame
			avcodec_decode_video2(codec_ctx, tmpFrame, &frameFinished, &packet);

			// Did we get a video frame?
			if (frameFinished) {
				// Convert the image from its native format to RGB
				sws_scale(sws_ctx, (uint8_t const * const *)tmpFrame->data,
					tmpFrame->linesize, 0, codec_ctx->height,
					rgbFrame->data, rgbFrame->linesize);

				// Save the frame to disk
				/*if (++i <= 60)
					SaveFrame(rgbFrame, codec_ctx->width,
						codec_ctx->height, i);*/
			}
		}

		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
	}

	avcodec_close(origCtx);

}

arda::Video::~Video()
{
	auto& format_ctx = m_internals->format_ctx;
	auto& avstream = m_internals->avstream;
	auto& codec_ctx = m_internals->codec_ctx;
	auto& codec_ctx_a = m_internals->codec_ctx_a;
	auto& codec = m_internals->codec;
	auto& tmpFrame = m_internals->m_tmpFrame;
	auto& rgbFrame = m_internals->m_rgbFrame;
	auto& alphaFrame = m_internals->m_alphaFrame;
	auto& rgbBuffer = m_internals->m_rgbBuffer;

	av_free(rgbBuffer);
	av_free(tmpFrame);
	av_free(rgbFrame);


	avcodec_close(codec_ctx);
	avcodec_close(codec_ctx_a);
	avformat_close_input(&format_ctx);
}
