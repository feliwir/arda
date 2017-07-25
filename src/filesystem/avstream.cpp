#include "avstream.hpp"
#include "stream.hpp"
#include <functional>
extern "C"
{
#include <libavformat/avio.h>
}

arda::AvStream::AvStream(std::shared_ptr<IStream> stream)
	: m_ctx(nullptr)
{
	m_ctx = avio_alloc_context(m_buffer,
								m_bufSize, 0,
								&stream,
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
}

int arda::AvStream::ReadFunc(void* ptr, uint8_t* buf, int buf_size)
{
	auto stream = *reinterpret_cast<std::shared_ptr<IStream>*>(ptr);
	int bytesRead = 0;

	bytesRead = stream->read(reinterpret_cast<char*>(buf), buf_size);
	return bytesRead;
}

// whence: SEEK_SET, SEEK_CUR, SEEK_END (like fseek) and AVSEEK_SIZE
int64_t arda::AvStream::SeekFunc(void* ptr, int64_t pos, int whence)
{
	// Quelle Abfragen:
	auto stream = *reinterpret_cast<std::shared_ptr<IStream>*>(ptr);

	IStream::SeekOrigin origin;
	switch (whence)
	{
	case 0:
		origin = IStream::BEGIN;
	case 1:
		origin = IStream::CURRENT;
	case 2:
		origin = IStream::END;
	}

	stream->seek(pos, origin);

	// Return the new position:
	return stream->getPosition();
}