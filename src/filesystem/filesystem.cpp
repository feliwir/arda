#include "filesystem.hpp"
#include "../core/config.hpp"
#include "../core/exception.hpp"
#include "../threading/pool.hpp"
#include "bigarchive.hpp"
#include "directory.hpp"
#include "file.hpp"
#include "filestream.hpp"
#include "stream.hpp"
#include <iostream>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

arda::FileSystem::FileSystem(Config& config)
{
	m_root = config.getRootDir();

	if (!fs::exists(m_root) || !fs::is_directory(m_root))
	{
		config.setRoot(".");
		m_root = ".";
	}
	
	//root dir of our VFS
	m_vfsRoot = std::make_shared<Directory>();

	//List all big archives
	fs::recursive_directory_iterator end;
	fs::recursive_directory_iterator iter(m_root);
	std::vector<std::string> regular;

	//parallelize the loading of big archives
	concurrent::ThreadPool<4> pool;

	for (iter; iter != end; ++iter)
	{
		// If it's not a directory, list it. If you want to list directories too, just remove this check.
		if (fs::is_regular_file(iter->path())) 
		{
			std::string path = iter->path().string();

			// assign current file name to current_file and echo it out to the console.
			if (iter->path().extension() == ".big")
				pool.AddJob([this,path]() {
				AddArchive(path);
				});

			else
				regular.push_back(path);				
		}
	}

	//perform this locally
	for (const auto& r : regular)
	{
		AddFile(r);
	}

	//wait for the pool
	pool.WaitAll();
}

arda::FileSystem::~FileSystem()
{

}

std::shared_ptr<arda::IStream> arda::FileSystem::getStream(const std::string & path) const
{
	auto dir = m_vfsRoot;
	std::shared_ptr<File> file = nullptr;
	
	for (auto part : fs::path(path))
	{
		auto entry = dir->getEntry(part.string());
		
		if (entry == nullptr)
		{
			std::cout << "Invalid path: " << path << std::endl;
			return nullptr;
		}
			

		if (IEntry::isDirectory(*entry))
			dir = std::static_pointer_cast<Directory>(entry);
		else
			file = std::static_pointer_cast<File>(entry);
	}

	if(file==nullptr)
		throw arda::RuntimeException("Not a valid file: " + path);

	return file->getStream();
}

std::shared_ptr<arda::IEntry> arda::FileSystem::getEntry(const std::string & path) const
{
	auto dir = m_vfsRoot;
	std::shared_ptr<IEntry> result = dir;

	for (auto part : fs::path(path))
	{
		auto entry = dir->getEntry(part.string());
		result = entry;
		if (IEntry::isDirectory(*entry))
		{
			dir = std::static_pointer_cast<Directory>(entry);
			result = dir;
		}			
	}

	if (result == nullptr)
		throw arda::RuntimeException("Not a valid entry: " + path);

	return result;
}

std::map<std::string, std::shared_ptr<arda::IEntry>> arda::FileSystem::listDirectory(const std::string & path) const
{
	auto dir = m_vfsRoot;

	for (auto part : fs::path(path))
	{
		auto entry = dir->getEntry(part.string());
		if (IEntry::isDirectory(*entry))
			dir = std::static_pointer_cast<Directory>(entry);
		else
			throw arda::RuntimeException("Not a valid directory: " + path);
	}
	
	return dir->getEntries();
}

void arda::FileSystem::AddArchive(const std::string& path)
{
	auto big = std::make_shared<BigArchive>(path);
	auto& entries = big->getEntries();

	for (auto& entry : big->getEntries())
	{
		m_vfsRoot->insertFile(entry.first, entry.second);
	}

	m_archives.push_back(big);
}

void arda::FileSystem::AddFile(const std::string& path)
{
	std::string relative = fs::relative(path, m_root).string();
	std::replace(relative.begin(), relative.end(), '\\', '/');
	std::transform(relative.begin(), relative.end(), relative.begin(), ::tolower);
	m_vfsRoot->insertFile(relative, std::make_shared<FileStream>(path));
}