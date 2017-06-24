#pragma once
#include <map>
#include <vector>
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
	//private:
	//	const static std::map<std::string, Block::BlockType> m_blockOpenings;
	//	const static std::map<std::string, Preprocessor::Command> m_ppCommands;
	public:
		void Parse(const std::string& content,const std::string& path,const FileSystem& fs);
		void Parse(std::shared_ptr<IStream> stream, const std::string& path,const FileSystem& fs);
	};
}