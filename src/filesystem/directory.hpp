#pragma once
#include <map>
#include <memory>
#include <string>
#include <mutex>
#include <algorithm>
#include "entry.hpp"
 
namespace arda
{
	class IStream;

	class Directory final : public IEntry
	{
	public:
		Directory(std::shared_ptr<IEntry> parent=nullptr);

		void InsertSelf();

		inline const std::map<std::string, std::shared_ptr<IEntry>>& GetEntries()
		{
			return m_entries;
		}

		inline std::shared_ptr<IEntry> GetEntry(const std::string& path)
		{
			auto it = m_entries.find(path);
			if (it == m_entries.end())
				return nullptr;
			else
				return it->second;
		}

		void InsertFile(const std::string& path,std::shared_ptr<IStream> stream);

	protected:
		std::mutex m_mutex;
		std::map<std::string, std::shared_ptr<IEntry>> m_entries;
	};
}