#include "lexer.hpp"
#include <sstream>
#include <algorithm>
#include <locale>

arda::TokenStream& arda::Lexer::Lex(const std::string & source, const std::string & filename)
{
	TokenStream stream(filename);

	std::string line;
	std::stringstream sourcestream;
	sourcestream << source;

	while (std::getline(sourcestream, line))
	{
		//remove all \r
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

		//replace tabs with spaces
		std::replace(line.begin(), line.end(), '\t', ' ');


	}

	return stream;
}

void arda::Lexer::SkipWhitespaces(const std::string & str, int& pos)
{
	while (isspace(str[pos]))
		++pos;
}
