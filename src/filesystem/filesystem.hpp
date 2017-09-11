#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include <sstream>

namespace arda
{
	class Config;
	class BigArchive;
	class IStream;
	class IEntry;
	class Directory;

	class FileSystem
	{
	public: 
		FileSystem(Config& config);
		~FileSystem();

		std::shared_ptr<IStream> GetStream(const std::string& path) const;
		std::shared_ptr<IEntry> GetEntry(const std::string& path) const;

		std::map<std::string, std::shared_ptr<IEntry>> ListDirectory(const std::string& path) const;
	private:
		void AddArchive(const std::string& path);
		void AddFile(const std::string& path);

		std::string m_root;
		std::shared_ptr<Directory> m_vfsRoot;
		std::vector<std::shared_ptr<BigArchive>> m_archives;
	};
}