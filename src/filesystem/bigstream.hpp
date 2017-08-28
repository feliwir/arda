#pragma once
#include "stream.hpp"
#include <string>
#include <memory>

namespace arda
{
	class BigArchive;

	class BigStream : public IStream
	{
	public:
		BigStream(int size, int offset, BigArchive* archive);

		// Inherited via IStream
		virtual void seek(int offset, SeekOrigin origin) override;
		virtual unsigned int read(char * buffer, size_t numBytes) override;

		inline int getOffset() { return m_offset; }
	protected:
		BigArchive* m_archive;
		int m_offset;
	};
}