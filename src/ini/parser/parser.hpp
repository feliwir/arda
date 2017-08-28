#pragma once
#include <map>
#include <memory>
#include <stack>
#include <sstream>
#include "token.hpp"

namespace arda
{
	class ParsingContext;
	class IStream;
	class FileSystem;
	class Ini;
	class TokenStream;

	class Parser
	{
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

		std::string PopString();
	private:
		std::stack<Token> m_arguments;
		Ini& m_ini;

	

	};
}