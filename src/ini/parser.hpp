#pragma once
#include <map>
#include <memory>
#include <sstream>

namespace arda
{
	class IStream;
	class FileSystem;

	class Parser
	{
	private:
		enum State
		{
			NO_BLOCK	= 0,
			IN_BLOCK	= 1,
		};

	public:
		void Parse(std::shared_ptr<IStream> stream, const std::string& path, const FileSystem& fs);
		

		void ClearCache();
	private:
		void Parse(std::shared_ptr<IStream> stream, const std::string& path, const FileSystem& fs, std::map<std::string, std::string>& macros);
		void Parse(const std::string& content, const std::string& path, const FileSystem& fs,std::map<std::string,std::string>& macros);
		//cache for macros and parsed files
		std::map<std::string, std::map<std::string, std::string>> m_cache;
	};
}