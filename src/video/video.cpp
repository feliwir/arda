#include "video.hpp"
#include "../filesystem/avstream.hpp"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

namespace arda
{
	class VideoInternals
	{
	public:
		std::unique_ptr<AvStream> avstream;
		AVCodec codec;
		AVCodecContext codec_ctx;
		AVFormatContext* format_ctx;
	};
}

arda::Video::Video(std::shared_ptr<IStream> stream) :
	m_internals(std::make_unique<VideoInternals>())
{
	av_register_all();
	m_internals->avstream = std::make_unique<AvStream>(stream);
	m_internals->format_ctx = avformat_alloc_context();
	m_internals->avstream->Attach(m_internals->format_ctx);

	avformat_open_input(&m_internals->format_ctx, "", 0, 0);
}

arda::Video::~Video()
{
}
