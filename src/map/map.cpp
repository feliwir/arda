#include "map.hpp"
#include "../filesystem/stream.hpp"
#include "../filesystem/util.hpp"
#include "../filesystem/refpackstream.hpp"
#include <iostream>

arda::Map::Map(std::shared_ptr<IStream> stream)
{
	Load(stream);
}

arda::Map::~Map()
{

}

bool arda::Map::Load(std::shared_ptr<IStream> stream)
{
	uint32_t flag = util::read<uint32_t>(stream);
	switch (flag)
	{
	case 1884121923U:
		std::cout << "map is uncompressed " << std::endl;
		break;

	case 5390661U: // "EAR\0"
		std::cout << "### Map is in compressed RefPack format, decompressing..." << std::endl;
		m_uncompressed_size = util::read<uint32_t>(stream);
		new RefPackStream(stream);
		break;

	default: // "ZL5\0"
		std::cout << "### Unknow map format, not supported" << std::endl;
		return false;
	}
	return true;
}