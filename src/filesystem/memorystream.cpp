#include "memorystream.hpp"

arda::MemoryStream::MemoryStream(std::shared_ptr<IStream> stream) : IStream(),
	m_buffer(nullptr)
{
	//seek to start
	stream->Seek(0, BEGIN);

	m_size = stream->GetSize();

	m_buffer = new uint8_t[m_size];

	stream->Read(reinterpret_cast<char*>(m_buffer), m_size);
}

arda::MemoryStream::~MemoryStream()
{
	if (m_buffer)
		delete[] m_buffer;
}

void arda::MemoryStream::Seek(int offset, SeekOrigin origin)
{
	switch (origin)
	{
	default:
	case BEGIN:
		m_position = offset;
		break;
	case CURRENT:
		m_position += offset;
		break;
	case END:
		m_position = m_size += offset;
		break;
	}
}

unsigned int arda::MemoryStream::Read(char * buffer, size_t numBytes)
{
	int shouldRead = (numBytes + m_position > m_size) ? m_size - m_position : numBytes;
	std::memcpy(buffer, m_buffer + m_position, shouldRead);
	m_position += shouldRead;
	return shouldRead;
}
