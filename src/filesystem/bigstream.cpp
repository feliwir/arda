#include "bigstream.hpp"
#include "bigarchive.hpp"

arda::BigStream::BigStream(int size,int offset, 
	BigArchive* archive) 
	: m_archive(nullptr)
{
	m_archive = archive;
	m_size = size;
	m_offset = offset;
	m_position = 0;
}

void arda::BigStream::Seek(int offset, SeekOrigin origin) 
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

unsigned int arda::BigStream::Read(char * buffer, size_t numBytes)
{
	int bytesRead = m_archive->read(buffer, m_offset + m_position, numBytes);
	m_position += bytesRead;
	return bytesRead;
}