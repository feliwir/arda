#include "map.hpp"
#include "../filesystem/stream.hpp"
#include "../filesystem/util.hpp"
#include "../filesystem/refpackstream.hpp"
#include "../core/debugger.hpp" 
#include <iostream>

using namespace std::string_literals;

arda::Map::Map(std::shared_ptr<IStream> stream)
{
	Load(stream);
}

arda::Map::~Map()
{

}

bool arda::Map::Load(std::shared_ptr<IStream> stream)
{
	std::string magic;
	for (int i = 0; i < 4; ++i)
		magic += stream->Get();
	

	if (magic == std::string("EAR\0"s))
	{
		ARDA_LOG("Map is in compressed RefPack format, decompressing...");
		m_uncompressed_size = util::Read<uint32_t>(stream);
		return Load(std::make_shared<RefPackStream>(stream));
	}
	else if (magic != std::string("CkMp"s))
	{
		//bad
		return false;
	}

	//fine 
	return true;

}