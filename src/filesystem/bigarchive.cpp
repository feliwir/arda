#include "bigarchive.hpp"
#include "bigstream.hpp"
#include "../core/exception.hpp"
#include "util.hpp"
#include <algorithm>

arda::BigArchive::BigArchive(const std::string & file)
{
	m_stream.open(file, std::ios::in | std::ios::binary);
	if (m_stream.fail())
		throw RuntimeException("Failed to open bigarchive. File: " + file);

	m_path = file;
	m_stream.seekg(0, std::ios::end);
	m_size = m_stream.tellg();
	m_stream.seekg(0, std::ios::beg);

	std::string magic;
	for (int i = 0; i < 4; ++i)
		magic += m_stream.get();

	if (magic == std::string("BIGF"))
		m_version = CC;
	else if (magic == std::string("BIG4"))
		m_version = BFME;
	else
		throw RuntimeException("Invalid BigArchive - unknown magic: "+magic+". File: " + file);

	int size = util::read<uint32_t>(m_stream);
	if(size!=m_size)
		throw RuntimeException("BigArchve internal size doesn't match real size. File: " + file);

	int numEntries = util::reverse(util::read<uint32_t>(m_stream));
	uint32_t first = util::reverse(util::read<uint32_t>(m_stream));

	for (auto i = 0; i < numEntries; i++)
	{
		uint32_t off = util::reverse(util::read<uint32_t>(m_stream));
		uint32_t size = util::reverse(util::read<uint32_t>(m_stream));
		auto stream = std::make_shared<BigStream>(size,off,this);

		auto name = util::readString(m_stream);
		std::replace(name.begin(), name.end(), '\\', '/');
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		m_entries[name] = stream;
	}
}

int arda::BigArchive::read(char * buffer, int offset, int size)
{
	m_stream.seekg(offset, std::ios::beg);
	m_stream.read(buffer, size);
	return m_stream.gcount();
}
