#pragma once

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

		virtual void seek(int offset, SeekOrigin origin) = 0;
		virtual unsigned int read(char* buffer,size_t numBytes) = 0;
	protected:
		int m_position;
		int m_size; 
	};
}