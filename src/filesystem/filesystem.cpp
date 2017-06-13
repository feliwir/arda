#include "filesystem.hpp"
#include "../core/config.hpp"
#include "filestream.hpp"
#include "stream.hpp"
#include "bigarchive.hpp"
#include <boost/filesystem.hpp>
#include <iostream>


namespace fs = boost::filesystem;

arda::FileSystem::FileSystem(Config& config)
{
	m_root = config.getRootDir();

	if (!fs::exists(m_root) || !fs::is_directory(m_root))
	{
		config.setRoot(".");
		m_root = ".";
	}
		
	//List all big archives
	fs::recursive_directory_iterator end;
	fs::recursive_directory_iterator iter(m_root);

	for (iter; iter != end; ++iter)
	{
		// If it's not a directory, list it. If you want to list directories too, just remove this check.
		if (fs::is_regular_file(iter->path())) 
		{
			// assign current file name to current_file and echo it out to the console.
			if (iter->path().extension() == ".big")
			{
				AddArchive(iter->path().string());
			}
			else
			{
				AddFile(iter->path().string());
			}
		}
	}

	std::cout << "Finished Filesystem" << std::endl;
}

arda::FileSystem::~FileSystem()
{

}

void arda::FileSystem::AddArchive(const std::string& path)
{
	auto big = std::make_shared<BigArchive>(path);
	auto& entries = big->getEntries();
	m_entries.insert(entries.begin(), entries.end());
	m_archives.push_back(big);
}

void arda::FileSystem::AddFile(const std::string& path)
{
	std::string relative = fs::relative(path, m_root).string();
	std::replace(relative.begin(), relative.end(), '\\', '/');
	std::transform(relative.begin(), relative.end(), relative.begin(), ::tolower);
	m_entries[relative] = std::make_shared<FileStream>(path);
}