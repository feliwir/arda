#pragma once
#include "stream.hpp"
#include <memory>

namespace arda
{
	class MemoryStream final : public IStream
	{
	public:
		MemoryStream(std::shared_ptr<IStream> stream);
		~MemoryStream();

		// Geerbt über IStream
		virtual void Seek(int offset, SeekOrigin origin) override;
		virtual unsigned int Read(char * buffer, size_t numBytes) override;

		inline uint8_t* GetBuffer()
		{
			return m_buffer;
		}
	private:
		uint8_t* m_buffer;
	};
}