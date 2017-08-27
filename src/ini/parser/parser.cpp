#include "parser.hpp"
#include "../core/exception.hpp"
#include "../template.hpp"
#include "../ini.hpp"
#include "token.hpp"
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>

arda::Parser::Parser(Ini & ini) : m_ini(ini)
{
}

void arda::Parser::Parse(std::shared_ptr<ParsingContext> context)
{
	auto stream = context->GetTokenStream();
	stream->Start();

	State s = NO_BLOCK;

	for (Token t = stream->GetToken(); t.Type != Token::EndOfFile; t = stream->GetToken())
	{
		switch (s)
		{
		case BLOCK_OPEN:
		case NO_BLOCK:
			BlockOpen(stream, s);
			break;
		case IN_BLOCK:
			InBlock(stream, s);
			break;
		case PROP_SET:
			break;
		}
	}
}

void arda::Parser::BlockOpen(std::shared_ptr<TokenStream> stream, State& s)
{
	Token t = stream->Current();

	switch (s)
	{
	case NO_BLOCK:
		if (t.Type == Token::StringLiteral)
		{
			m_arguments.push(t);
			s = BLOCK_OPEN;
		}
		break;
	case BLOCK_OPEN:
		if (t.Type == Token::StringLiteral)
		{
			m_arguments.push(t);
		}

		if (t.Type == Token::EndOfLine)
		{
			std::string type, name;
			if (m_arguments.size() == 2)
				name = PopString();

			type = PopString();
			s = IN_BLOCK;
		}
		break;
	}
}

void arda::Parser::InBlock(std::shared_ptr<TokenStream> stream, State & s)
{
	Token t = stream->Current();
	std::string str;

	switch (t.Type)
	{
	case Token::StringLiteral:
		str = std::get<std::string>(t.Value);
		if (str == "End") { s = NO_BLOCK; }
		else
		{
			if (stream->Peek().Type == Token::Equals)
			{
				s = PROP_SET;
				stream->GetToken();
				m_arguments.push(t);
			}
		}
		break;
	}

}

std::string arda::Parser::PopString()
{
	std::string result = std::get<std::string>(m_arguments.top().Value);
	m_arguments.pop();
	return result;
}
