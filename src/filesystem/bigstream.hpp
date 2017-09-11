#pragma once
#include "stream.hpp"
#include <string>
#include <memory>

namespace arda
{
	class BigArchive;

	class BigStream final : public IStream
	{
	public:
		BigStream(int size, int offset, BigArchive* archive);

		// Inherited via IStream
		virtual void Seek(int offset, SeekOrigin origin) override;
		virtual unsigned int Read(char * buffer, size_t numBytes) override;

		inline int GetOffset() { return m_offset; }
	protected:
		BigArchive* m_archive;
		int m_offset;
	};
}