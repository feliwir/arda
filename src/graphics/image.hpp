#pragma once
#include <string>
#include <memory>
#include <stdint.h>
#include <gli/gli.hpp>

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
		Image();
		Image(std::shared_ptr<IStream> stream);
		~Image();
		
		inline gli::texture& GetTexture()	{ return m_img;	}
		inline int GetWidth()				{ return m_img.extent().x;	}
		inline int GetHeight()				{ return m_img.extent().y;	}
		inline uint8_t* GetBuffer()			{ return static_cast<uint8_t*>(m_img.data());  }

		bool Load(std::shared_ptr<IStream> stream);
	private:
		Magic GetMagic(uint8_t* memory);
	private:
		gli::texture m_img;
	};
}