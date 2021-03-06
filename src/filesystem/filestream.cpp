#include "filestream.hpp"
#include "../core/exception.hpp"
#include <boost/filesystem.hpp>

arda::FileStream::FileStream(const std::string& file)
{
	m_path = file;
	m_position = 0;
	m_size = static_cast<int>(boost::filesystem::file_size(file));
}

void arda::FileStream::checkOpen()
{
	if (!m_stream.is_open())
	{
		m_stream.open(m_path, std::ios::in | std::ios::binary);
		if (m_stream.fail())
			throw RuntimeException("Failed to open filestream. File: " + m_path);
	}
}

void arda::FileStream::Seek(int offset, SeekOrigin origin)
{
	checkOpen();

	std::ios::seekdir way = std::ios::beg;
	switch (origin)
	{
	default:
	case BEGIN:
		m_position = offset;
		way = std::ios::beg;
		break;
	case CURRENT:
		m_position += offset;
		way = std::ios::cur;
		break;
	case END:
		m_position = m_size += offset;
		way = std::ios::end;
		break;
	}
	m_stream.seekg(offset, way);
}

unsigned int arda::FileStream::Read(char * buffer, size_t numBytes)
{
	checkOpen();
	m_stream.read(buffer, numBytes);
	unsigned int bytesRead = m_stream.gcount();
	m_position += bytesRead;
	return bytesRead;
}
