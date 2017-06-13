#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <algorithm>

namespace arda
{
	class Config;
	class BigArchive;
	class IStream;

	class FileSystem
	{
	public: 
		FileSystem(Config& config);
		~FileSystem();

		inline std::map<std::string, std::shared_ptr<IStream>> listDirectory(const std::string& dir)
		{
			std::map<std::string, std::shared_ptr<IStream>> content;


			for (const auto& e : m_entries)
			{
				if (e.first.find(dir) == 0)
					content.insert(e);
			}

			return content;
		}
		
		
		inline std::shared_ptr<IStream> getFile(const std::string& file)
		{
			auto it = m_entries.find(file);
			if (it != m_entries.end())
				return it->second;
			else
				return nullptr;
		}

	private:
		void AddArchive(const std::string& path);
		void AddFile(const std::string& path);

		std::string m_root;
		std::map<std::string, std::shared_ptr<IStream>> m_entries;
		std::vector<std::shared_ptr<BigArchive>> m_archives;
	};
}