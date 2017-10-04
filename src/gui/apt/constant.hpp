#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stdint.h>

namespace arda
{
	class IStream;

	class Constant
	{
	public:
		enum Type
		{
			UNDEF = 0,
			STRING = 1,
			NUMBER = 4
		};

		struct Entry
		{
			Type Type;
			std::string StringValue;
			uint32_t NumberValue;
			Entry();
		};
	public:
		Constant(std::shared_ptr<IStream> stream);


		inline Entry GetItem(const uint32_t index)
		{
			if (index > m_items.size())
				return Entry();

			return m_items[index];
		}

		inline const uint32_t GetAptOffset()
		{
			return m_aptOffset;
		}

		inline const uint32_t GetItemCount()
		{
			return m_itemCount;
		}


	private:
		uint32_t m_aptOffset;
		uint32_t m_itemCount;
		std::vector<Entry> m_items;
	};
}