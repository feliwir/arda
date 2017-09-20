#pragma once
#include <fstream>
#include <string>
#include <stdint.h>
#include <array>
#include <algorithm>
#include "stream.hpp"

namespace arda
{
	namespace util
	{
		template<class T,int N = 4>
		inline bool CheckFourCC(uint8_t* buffer, std::array<T,N> check)
		{
			static_assert(sizeof(T) == 1, "FourCC check only accepts data types of 1 byte size!");

			return std::equal(std::begin(check), std::begin(check) + N, buffer,buffer+N);
		}

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