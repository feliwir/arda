#pragma once
#include <memory>

namespace arda
{
	class IStream;

	class Map
	{
	public:
		Map(std::shared_ptr<IStream> stream);
		~Map();

		bool Load(std::shared_ptr<IStream> stream);
	private:
		uint32_t m_uncompressed_size;
	};
}