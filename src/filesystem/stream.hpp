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
		virtual int read(char* buffer,int numBytes) = 0;
	protected:
		int m_position;
		int m_size; 
	};
}