#include "filesystem.hpp"
#include "../core/config.hpp"
#include "../core/exception.hpp"
#include "../core/application.hpp"
#include "../core/debugger.hpp"
#include "../core/global.hpp"
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
	m_root = config.GetRootDir();

	if (!fs::exists(m_root) || !fs::is_directory(m_root))
	{
		config.SetRoot(".");
		m_root = ".";
	}
	
	//root dir of our VFS
	m_vfsRoot = std::make_shared<Directory>();

	//List all big archives
	fs::recursive_directory_iterator end;
	fs::recursive_directory_iterator iter(m_root);
	std::vector<std::string> regular;

	//parallelize the loading of big archives
	auto& pool = GetGlobal().GetThreadPool();

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

std::shared_ptr<arda::IStream> arda::FileSystem::GetStream(const std::string & path) const
{
	auto dir = m_vfsRoot;
	std::shared_ptr<File> file = nullptr;
	std::string p = path;
	std::transform(p.begin(), p.end(), p.begin(), ::tolower);

	for (auto part : fs::path(p))
	{
		auto entry = dir->GetEntry(part.string());
		
		if (entry == nullptr)
		{
			ARDA_LOG("Invalid path: " + path);
			return nullptr;
		}
			

		if (IEntry::isDirectory(*entry))
			dir = std::static_pointer_cast<Directory>(entry);
		else
			file = std::static_pointer_cast<File>(entry);
	}

	if(file==nullptr)
		throw arda::RuntimeException("Not a valid file: " + path);

	return file->GetStream();
}

std::shared_ptr<arda::IEntry> arda::FileSystem::GetEntry(const std::string & path) const
{
	auto dir = m_vfsRoot;
	std::shared_ptr<IEntry> result = dir;
	std::string p = path;
	std::transform(p.begin(), p.end(), p.begin(), ::tolower);

	for (auto part : fs::path(p))
	{
		auto entry = dir->GetEntry(part.string());
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

std::map<std::string, std::shared_ptr<arda::IEntry>> arda::FileSystem::ListDirectory(const std::string & path) const
{
	auto dir = m_vfsRoot;

	for (auto part : fs::path(path))
	{
		auto entry = dir->GetEntry(part.string());
		if (IEntry::isDirectory(*entry))
			dir = std::static_pointer_cast<Directory>(entry);
		else
			throw arda::RuntimeException("Not a valid directory: " + path);
	}
	
	return dir->GetEntries();
}

void arda::FileSystem::AddArchive(const std::string& path)
{
	auto big = std::make_shared<BigArchive>(path);
	auto& entries = big->GetEntries();

	for (auto& entry : big->GetEntries())
	{
		m_vfsRoot->InsertFile(entry.first, entry.second);
	}

	m_access.lock();
	m_archives.push_back(big);
	m_access.unlock();
}

void arda::FileSystem::AddFile(const std::string& path)
{
	std::string relative = fs::relative(path, m_root).string();
	std::replace(relative.begin(), relative.end(), '\\', '/');
	std::transform(relative.begin(), relative.end(), relative.begin(), ::tolower);
	m_vfsRoot->InsertFile(relative, std::make_shared<FileStream>(path));
}