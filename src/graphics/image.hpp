#pragma once
#include <string>
#include <memory>
#include <stdint.h>

namespace arda
{
	class IStream;

	class Image
	{
	private:
		enum Magic
		{
			JPEG	= 0,
			TARGA	= 1,
			DDS		= 2,
			
		};
	public:
		enum Format
		{
			RGBA		= 0,
			RGB			= 1,
			COMPRESSED	= 2
		};
	public:
		Image();
		Image(std::shared_ptr<IStream> stream);
		~Image();

		inline Format GetFormat()	{ return m_format;	}
		inline int GetWidth()		{ return m_width;	}
		inline int GetHeight()		{ return m_height;	}
		inline uint8_t* GetBuffer()	{ return m_buffer;  }

		bool Load(std::shared_ptr<IStream> stream);
	private:
		Magic GetMagic(uint8_t* memory);

	private:
		int m_bpp;
		int m_width;
		int m_height;
		int m_size;
		Format m_format;
		uint8_t* m_buffer;
	};
}