#pragma once
#include <string>
#include "tokenstream.hpp"

namespace arda
{
	class Lexer
	{
	public:
		static TokenStream& Lex(const std::string& source, const std::string& filename);
	private:
		static void SkipWhitespaces(const std::string& str, int& pos);
	};
}