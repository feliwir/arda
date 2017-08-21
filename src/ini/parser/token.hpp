#pragma once
#include <string>
#include <variant>

namespace arda
{
	struct Token
	{
		enum TokenType
		{
			Unknown,
			Identifier,
			Equals,
			IntegerLiteral,
			FloatLiteral,
			PercentLiteral,
			StringLiteral,
			UnicodeLiteral,
			Hashtag,
			Colon,
			Comma,
			Plus,
			Minus,
			EndOfLine,
			EndOfFile,
		};

		struct TokenPosition
		{
			int Line;
			int Column;
		};

		Token();
		Token(TokenType type);
		Token(const std::string& str,bool unicode = false);
		Token(const long long num);
		Token(const double num, bool percent = false);

		TokenPosition Position;
		TokenType Type;
		std::variant<std::string,long long, double> Value;
	};

}