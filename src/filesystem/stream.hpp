#pragma once
#include <string>

namespace arda
{
	class IStream
	{
	public:
		enum SeekOrigin
		{
			BEGIN,
			CURRENT,
			END
		};
	public:
		inline int GetSize() { return m_size; }
		inline int GetPosition() { return m_position; }

		inline int Get()
		{
			char byte[1];
			Read(byte, 1);
			return byte[0];
		}

		virtual void Seek(int offset, SeekOrigin origin) = 0;
		virtual unsigned int Read(char* buffer,size_t numBytes) = 0;

		inline std::string ReadAll()
		{
			std::string buffer(GetSize(), ' ');
			Read(&buffer[0], GetSize());
			return buffer;
		}

	protected:
		int m_position = 0;
		int m_size = 0; 
	};
}