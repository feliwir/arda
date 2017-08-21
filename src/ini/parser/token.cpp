#include "token.hpp"

arda::Token::Token()
{
}

arda::Token::Token(TokenType type)
{
	Type = type;
}

arda::Token::Token(const std::string & str,bool unicode)
{
	Value = str;

	if (unicode)
		Type = UnicodeLiteral;
	else
		Type = StringLiteral;
}

arda::Token::Token(const long long num)
{
	Value = num;
	
	Type = IntegerLiteral;
}

arda::Token::Token(const double num, bool percent)
{
	Value = num;

	if(percent)
		Type = PercentLiteral;
	else
		Type = FloatLiteral;
}
