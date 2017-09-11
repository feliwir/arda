#pragma once
#include <fstream>
#include <string>
#include <stdint.h>
#include "stream.hpp"

namespace arda
{
	namespace util
	{

		inline uint32_t Reverse(uint32_t v)
		{
			return (v << 24) | (v << 8 & 0xff0000) | (v >> 8 & 0xff00) | (v >> 24);
		}

		template <class T>
		inline T Read(std::fstream& stream)
		{
			T result;
			stream.read(reinterpret_cast<char*>(&result), sizeof(T));
			return result;
		}

		inline std::string ReadString(std::fstream& stream)
		{
			std::string buffer;
			char c;
			while ((c = stream.get()) != '\0') {
				buffer += c;
			}

			return buffer;
		}

		template <class T>
		inline T Read(std::shared_ptr<IStream> stream)
		{
			T result;
			stream->Read(reinterpret_cast<char*>(&result), sizeof(T));
			return result;
		}
	}
}