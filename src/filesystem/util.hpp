#pragma once
#include <fstream>
#include <stdint.h>

namespace arda
{
	namespace util
	{
		inline uint32_t reverse(uint32_t v)
		{
			return (v << 24) | (v << 8 & 0xff0000) | (v >> 8 & 0xff00) | (v >> 24);
		}

		template <class T>
		inline T read(std::fstream& stream)
		{
			T result;
			stream.read(reinterpret_cast<char*>(&result), sizeof(T));
			return result;
		}

		inline std::string readString(std::fstream& stream)
		{
			std::string buffer;
			char c;
			while ((c = stream.get()) != '\0') {
				buffer += c;
			}

			return buffer;
		}
	}
}