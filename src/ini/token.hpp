#pragma once
#include <string>

namespace arda
{
	enum TokenType
	{
		Identifier,
		Equals,
		IntegerLiteral,
		FloatLiteral,
		PercentLiteral,
		Hashtag,
		Colon,
		Comma,
		EndOfLine,
		EndOfFile,
	};

	struct TokenPosition
	{
		int Line;
		int Column;
	};

	struct Token
	{
		TokenPosition Position;
		TokenType Type;
		std::string StringValue;
		int IntegerValue;
		float FloatValue;
	};

}