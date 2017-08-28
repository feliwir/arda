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
		inline int getSize() { return m_size; }
		inline int getPosition() { return m_position; }

		inline int get()
		{
			char byte[1];
			read(byte, 1);
			return byte[0];
		}

		virtual void seek(int offset, SeekOrigin origin) = 0;
		virtual unsigned int read(char* buffer,size_t numBytes) = 0;

		inline std::string readAll()
		{
			std::string buffer(getSize(), ' ');
			read(&buffer[0], getSize());
			return buffer;
		}

	protected:
		int m_position = 0;
		int m_size = 0; 
	};
}