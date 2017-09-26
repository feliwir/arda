#pragma once
#include "stream.hpp"
#include <string>
#include <fstream>

namespace arda
{
	class FileStream final : public IStream
	{
	public:
		FileStream(const std::string& file);

		// Inherited via IStream
		virtual void Seek(int offset, SeekOrigin origin) override;
		virtual unsigned int Read(char * buffer, size_t numBytes) override;

		void checkOpen();
	protected:
		std::fstream m_stream;
		std::string m_path;
	};
}