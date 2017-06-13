#include "tokenizer.hpp"
#include "../core/exception.hpp"
#include <string>
#include <algorithm>
#include <cctype>

inline std::string trim(const std::string& str,
	const std::string& whitespace = " ")
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

const std::map<std::string, arda::Block::BlockType> arda::Tokenizer::m_blockOpenings =
{
	{"Weapon",arda::Block::WEAPON}
};

const std::map<std::string, arda::Preprocessor::Command> arda::Tokenizer::m_ppCommands =
{
	{"#define",arda::Preprocessor::DEFINE},
	{"#include",arda::Preprocessor::INCLUDE},
	{"#multiply",arda::Preprocessor::MULTIPLY },
};

std::vector<std::shared_ptr<arda::IToken>> arda::Tokenizer::Parse(const std::string& ini)
{
	std::stringstream ss(ini);
	std::stringstream ls;

	std::vector<std::shared_ptr<arda::IToken>> tokens;
	std::map<std::string, std::string> macros;

	State cstate = NO_BLOCK;
	int lnum = 1;

	auto preprocess = [&]()
	{
		if (ls.peek() == '#')
		{
			std::string expr;
			ls >> expr;
			auto it = m_ppCommands.find(expr);
			if (it == m_ppCommands.end())
				throw RuntimeException("Unkown preprocessor directive!");

			auto cmd = it->second;
			switch (cmd)
			{
			case Preprocessor::DEFINE:
			{
				std::string macro;
				std::string value;
				ls >> macro >> value;
				macros[macro] = value;
			}
			break;
			default:
				throw RuntimeException("Unimplemented preprocessor directive!");
			}

			return true;
		}
		else
			return false;
	};

	std::string word;
	std::string line;

	//parse line by line
	while (std::getline(ss, line))
	{
		//remove everything after a comment
		line = line.substr(0, line.find(";"));

		//remove all whitespaces
		line = trim(line);

		//erase all control chars
		line.erase(std::remove_if(line.begin(), line.end(), std::iscntrl), line.end());

		//skip empty lines
		if (line.size() == 0)
		{
			++lnum;
			continue;
		}
			

		ls.clear();
		ls.str(line);
			
		//preprocessor instruction, if it was one we are done with the line
		if (preprocess())
		{
			++lnum;
			continue;
		}
			

		//next step we check if we have a valid block opening
		ls >> word;
		auto it = m_blockOpenings.find(word);

		if (it != m_blockOpenings.end())
		{
			if (cstate == IN_BLOCK)
				throw arda::RuntimeException("Failed INI parsing: Already inside a block! Missing end?");

			auto tok = std::make_shared<Block>();
			tok->TypeToken = IToken::BLOCK_BEG;
			tok->TypeBlock = it->second;
			ls >> word;
			tok->Name = word;
			tokens.push_back(tok);
			cstate = IN_BLOCK;
		}
	}

	return tokens;
}