#include "directory.hpp"
#include "file.hpp"
#include "stream.hpp"

arda::Directory::Directory(std::shared_ptr<IEntry> parent) : IEntry(DIRECTORY)
{	
	std::lock_guard<std::mutex> lock(m_mutex);
	m_entries[".."] = parent;
}

void arda::Directory::InsertSelf()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if(m_entries.find(".")==m_entries.end())
		m_entries["."] = shared_from_this();

}

void arda::Directory::InsertFile(const std::string& path, std::shared_ptr<IStream> stream)
{
	auto pos = path.find_first_of('/');

	//this is a file add it here
	if (pos == std::string::npos)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_entries[path] = std::make_shared<File>(stream);
	}
	//not a file
	else
	{
		auto dirName = path.substr(0, pos);

		m_mutex.lock();
		auto it = m_entries.find(dirName);

		//create a new directory
		if (it == m_entries.end())
		{
			std::shared_ptr<IEntry> dir = std::make_shared<Directory>(shared_from_this());
			it = m_entries.insert(it,std::pair<std::string, std::shared_ptr<IEntry>>(dirName, dir));
		}
		m_mutex.unlock();

		auto dir = std::static_pointer_cast<Directory>(it->second);
		dir->InsertSelf();
		auto remPath = path.substr(pos+1, path.size());
		dir->InsertFile(remPath, stream);
	}
}
