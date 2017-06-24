#pragma once
#include "stream.hpp"
#include <string>
#include <fstream>

namespace arda
{
	class FileStream : public IStream
	{
	public:
		FileStream(const std::string& file);

		// Inherited via IStream
		virtual void seek(int offset, SeekOrigin origin) override;
		virtual unsigned int read(char * buffer, size_t numBytes) override;

		void checkOpen();
	protected:
		std::fstream m_stream;
		std::string m_path;
	};
}