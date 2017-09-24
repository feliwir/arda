#include "parser.hpp"
#include "../core/exception.hpp"
#include "../ini.hpp"
#include "../blocks/all.hpp"
#include "token.hpp"
#include "block.hpp"
#include "context.hpp"
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>

const std::map<const std::string, arda::Parser::BlockConstruct> arda::Parser::m_constructors =
{
	{"GameData", Block::Create<ini::GameData>},
	{"Video", Block::Create<ini::Video> },
	{"Weapon", Block::Create<ini::Weapon>}
};

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
			PropSet(stream, s);
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

			CreateBlock(type, name);

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
		if (str == "End") 
		{ 
			if (m_block)
			{
				//TODO: Add block to library

				//clear
				s = NO_BLOCK;
				m_block = nullptr;
			}			
		}
		else
		{
			//PROPERTY
			if (stream->Peek().Type == Token::Equals)
			{
				s = PROP_SET;
				stream->GetToken();
				m_arguments.push(t);
			}
			//NUGGET or undefined Block
			else
			{
				int a = 0;
			}
		}
		break;
	}

}

void arda::Parser::PropSet(std::shared_ptr<TokenStream> stream, State & s)
{
	Token t = stream->Current();

	if (t.Type == Token::EndOfLine)
		CreateProperty(s);
	else
		m_arguments.push(t);
}

const std::string arda::Parser::PopString()
{
	std::string result = std::get<std::string>(m_arguments.top().Value);
	m_arguments.pop();
	return result;
}

const arda::Token arda::Parser::PopToken()
{
	Token result = m_arguments.top();
	m_arguments.pop();
	return result;
}

void arda::Parser::CreateBlock(const std::string & type, const std::string & name)
{
	auto it = m_constructors.find(type);

	if (it == m_constructors.end())
	{
		//std::cout << "Unknown block: " << type << std::endl;
		return;
	}
		
	m_block = it->second();
	m_block->Register(m_ini, name);
}

void arda::Parser::CreateProperty(State & state)
{
	std::vector<Token> arguments;
	std::string name;

	while (m_arguments.size() > 1)
		arguments.push_back(PopToken());

	name = PopString();

	if (m_block)
	{
		m_block->SetProperty(name, arguments);
		state = IN_BLOCK;
	}
	else
	{
		state = IN_BLOCK;
	} 
}
