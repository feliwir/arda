#include "avstream.hpp"
#include "stream.hpp"
#include <functional>
#include <cstring>

extern "C"
{
#include <libavformat/avio.h>
#include <libavformat/avformat.h>
}

arda::AvStream::AvStream(std::shared_ptr<IStream> stream)
	: m_ctx(nullptr),
	  m_stream(stream),
	  m_buffer(nullptr)
{
	m_buffer = new uint8_t[m_bufSize];
	m_ctx = avio_alloc_context(m_buffer,
								m_bufSize, 0,
								m_stream.get(),
								ReadFunc,
								NULL,
								SeekFunc);
}

arda::AvStream::~AvStream()
{
	if (m_ctx)
	{
		av_free(m_ctx);
		m_ctx = nullptr;
	}

	if (m_buffer)
	{
		delete[] m_buffer;
	}
}

int arda::AvStream::ReadFunc(void* ptr, uint8_t* buf, int buf_size)
{
	auto stream = reinterpret_cast<IStream*>(ptr);
	int bytesRead = 0;

	bytesRead = stream->read(reinterpret_cast<char*>(buf), buf_size);
	return bytesRead;
}

void arda::AvStream::Attach(AVFormatContext * ctx)
{
	ctx->pb = GetContext();
	ctx->flags = AVFMT_FLAG_CUSTOM_IO;
	
	// Determining the input format:
	std::memset(m_buffer, 0, m_bufSize);
	int readBytes = 0;
	int size = m_bufSize - AVPROBE_PADDING_SIZE;
	readBytes = m_stream->read(reinterpret_cast<char*>(m_buffer), size);
	m_stream->seek(0, IStream::BEGIN);

	// Now we set the ProbeData-structure for av_probe_input_format:
	AVProbeData probeData;
	probeData.buf = m_buffer;
	probeData.buf_size = readBytes;
	probeData.filename = "";
	probeData.mime_type = NULL;

	// Determine the input-format:
	ctx->iformat = av_probe_input_format(&probeData, 1);
}

// whence: SEEK_SET, SEEK_CUR, SEEK_END (like fseek) and AVSEEK_SIZE
int64_t arda::AvStream::SeekFunc(void* ptr, int64_t pos, int whence)
{
	// Quelle Abfragen:
	auto stream = reinterpret_cast<IStream*>(ptr);

	IStream::SeekOrigin origin;
	switch (whence)
	{
	case AVSEEK_SIZE:
		return stream->getSize();
		break;
	case SEEK_SET:
		origin = IStream::BEGIN;
		break;
	case SEEK_CUR:
		origin = IStream::CURRENT;
		break;
	case SEEK_END:
		origin = IStream::END;
		break;
	}

	stream->seek(pos, origin);

	// Return the new position:
	return stream->getPosition();
}