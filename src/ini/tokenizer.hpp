#pragma once
#include <map>
#include <vector>
#include <memory>
#include <sstream>

namespace arda
{
	struct IToken
	{
		enum TokenType
		{
			COMMENT		= 0,	//a simple comment
			PPROC		= 1,	//a preprocessor instruction
			BLOCK_BEG	= 2,	//a block beginning
			BLOCK_END	= 3,	//ends a block
			ASSIGNMENT	= 4
		};

		TokenType TypeToken;
	};


	struct Block : public IToken
	{
		enum BlockType
		{
			WEAPON = 0,
		};

		BlockType TypeBlock;
		std::string Name;
	};

	struct Preprocessor
	{
		enum Command
		{
			DEFINE		= 0,
			MULTIPLY	= 1,
			INCLUDE		= 2
		};
	};


	class Tokenizer
	{
	private:
		enum State
		{
			NO_BLOCK	= 0,
			IN_BLOCK	= 1,
		};
	private:
		const static std::map<std::string, Block::BlockType> m_blockOpenings;
		const static std::map<std::string, Preprocessor::Command> m_ppCommands;
	public:
		std::vector<std::shared_ptr<IToken>> Parse(const std::string& ini);
	};
}