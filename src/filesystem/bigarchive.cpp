#include "bigarchive.hpp"
#include "bigstream.hpp"
#include "../core/exception.hpp"
#include "util.hpp"
#include <algorithm>

using namespace std::string_literals;

arda::BigArchive::BigArchive(const std::string & file)
{
	m_stream.open(file, std::ios::in | std::ios::binary);
	if (m_stream.fail())
		throw RuntimeException("Failed to open bigarchive. File: " + file);

	m_path = file;
	m_stream.seekg(0, std::ios::end);
	m_size = static_cast<unsigned int>(m_stream.tellg());
	m_stream.seekg(0, std::ios::beg);

	std::string magic;
	for (int i = 0; i < 4; ++i)
		magic += m_stream.get();

	if (magic == std::string("BIGF"s))
		m_version = CC;
	else if (magic == std::string("BIG4"s))
		m_version = BFME;
	else
		throw RuntimeException("Invalid BigArchive - unknown magic: "+magic+". File: " + file);

	int size = util::Read<uint32_t>(m_stream);
	if(size!=m_size)
		throw RuntimeException("BigArchve internal size doesn't match real size. File: " + file);

	int numEntries = util::Reverse(util::Read<uint32_t>(m_stream));
	uint32_t first = util::Reverse(util::Read<uint32_t>(m_stream));

	for (auto i = 0; i < numEntries; i++)
	{
		uint32_t off = util::Reverse(util::Read<uint32_t>(m_stream));
		uint32_t size = util::Reverse(util::Read<uint32_t>(m_stream));
		auto stream = std::make_shared<BigStream>(size,off,this);

		auto name = util::ReadString(m_stream);
		std::replace(name.begin(), name.end(), '\\', '/');
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		m_entries[name] = stream;
	}
}

unsigned int arda::BigArchive::read(char * buffer, int offset, size_t size)
{
	std::lock_guard<std::mutex> lock(m_access);
	m_stream.seekg(offset, std::ios::beg);
	m_stream.read(buffer, size);
	return static_cast<unsigned int>(m_stream.gcount());
}
