#pragma once
#include <string>
#include <fstream>
#include <map>
#include <memory>

namespace arda
{
	class IStream;

	class BigArchive
	{
	public:
		enum Version
		{
			CC		= 0,
			BFME	= 1,
		};

		BigArchive(const std::string& file);

		inline std::map<std::string, std::shared_ptr<IStream>>& getEntries()
		{
			return m_entries;
		}

	protected:
		unsigned int read(char* buffer, int offset, size_t size);

		std::fstream m_stream;
		Version m_version;
		std::string m_path;
		unsigned int m_size;
		std::map<std::string, std::shared_ptr<IStream>> m_entries;

		friend class BigStream;
	};
}