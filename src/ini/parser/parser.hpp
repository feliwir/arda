#pragma once
#include <map>
#include <memory>
#include <stack>
#include <functional>
#include <sstream>
#include "token.hpp"

namespace arda
{
	class Block;
	class ParsingContext;
	class IStream;
	class FileSystem;
	class Ini;
	class TokenStream;

	class Parser
	{
		typedef std::function<std::shared_ptr<Block>(void)> BlockConstruct;

	private:
		enum State
		{
			NO_BLOCK,
			IN_BLOCK,
			BLOCK_OPEN,
			PROP_SET,
		};

	public:
		Parser(Ini& m_ini);
		void Parse(std::shared_ptr<ParsingContext> context);
	private:
		void BlockOpen(std::shared_ptr<TokenStream> t, State& s);
		void InBlock(std::shared_ptr<TokenStream> t, State& s);
		void PropSet(std::shared_ptr<TokenStream> t, State& s);

		const std::string PopString();
		const Token PopToken();

		void CreateBlock(const std::string& type, const std::string& name="");
		void CreateProperty(State& state);
	private:
		std::shared_ptr<Block> m_block;
		std::string m_blockName;
		std::stack<Token> m_arguments;
		static const std::map<const std::string, BlockConstruct> m_constructors;
		Ini& m_ini;

	

	};
}