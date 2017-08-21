#include "lexer.hpp"
#include <sstream>
#include <algorithm>
#include <codecvt>
#include <locale>
#include "../ini.hpp"
#include "../../filesystem/stream.hpp"

int arda::Lexer::cline = 0;

std::shared_ptr<arda::ParsingContext> arda::Lexer::Lex(std::shared_ptr<IStream> stream, 
	const std::string & filename,Ini& ini, FileSystem& fs)
{
	auto context = std::make_shared<ParsingContext>();
	
	for (const auto& include : ini.GetGlobalIncludes())
	{
		if (include != filename)
			context->GetIncludes().push_back(ini.GetContext(include,fs));
	}


	auto tokens = std::make_shared<TokenStream>(filename);

	std::string line;
	int pos;
	cline = 0;
	std::stringstream sourcestream;
	std::string source = stream->readAll();
	sourcestream << source;

	while (std::getline(sourcestream, line))
	{
		//remove all \r
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

		//replace tabs with spaces
		std::replace(line.begin(), line.end(), '\t', ' ');

		if (line.size() == 0)
			continue;

		//remove comments
		pos = line.find(';');
		if (pos != std::string::npos)
		{
			line = line.substr(0, pos);
		}

		if (CheckEol(line, tokens))
			continue;

		pos = line.find("//");
		if (pos != std::string::npos)
		{
			line = line.substr(0, pos);
		}

		if (CheckEol(line, tokens))
			continue;

		//remove trailing whitespaces
		line = trim(line);

		if (CheckEol(line, tokens))
			continue;


		pos = 0;

		while (pos < line.size())
		{
			Token tok = CreateToken(line, pos,context);
			
			if(tok.Type!=Token::Unknown)
				tokens->AddToken(tok);
		}

		AddEol(tokens,pos);
	}

	context->SetTokens(tokens);
	return context;
}

arda::Token arda::Lexer::CreateToken(const std::string & line, int & pos, std::shared_ptr<ParsingContext> context)
{
	bool parse = true;
	Token t;
	LexerMode mode = NORMAL;
	Token::TokenType type = Token::Unknown;
	std::string content;
	int col = pos;
	char c = 0;
	SkipWhitespaces(line,pos);

	while (parse)
	{
		c = line[pos];

		if (mode == UNICODE)
		{		
			if (c == '"')
				parse = false;
			else
			{
				content += c;
			}				
		}

		if (mode == NORMAL)
		{
			if (isdigit(c))
				mode = INTEGER;
			else if (isalpha(c))
				mode = STRING;
			else if (c == '"')
				mode = UNICODE;
			else
				mode = SINGLE_CHAR;
		}

		if (mode == INTEGER || mode == STRING || mode == FLOAT)
		{
			if ((mode == INTEGER|| mode == FLOAT) && c == '%')
			{
				mode = PERCENT;
				parse = false;
			}

			if (mode == INTEGER && c == '.')
			{
				mode = FLOAT;
			}

			if (isspace(c))
				parse = false;
			else
				content += c;		
		}
		
		else if (mode == SINGLE_CHAR)
		{
			switch (c)
			{
			case '=':
				type = Token::Equals;
				break;
			case '+':
				type = Token::Plus;
				break;
			case '-':
				type = Token::Minus;
				break;
			case '%':
				break;
			case '#':
				//PREPROCESSOR FUN
				Preprocess(line, pos,context);
				break;
			default:
			{
				int a = 0;
			}
				break;
			}
				
			parse = false;
		}
		
		++pos;

		if (pos == line.size())
			parse = false;
	}

	switch (mode)
	{
	case STRING:
		t = Token(content);
		break;
	case INTEGER:
		t = Token(std::stoll(content));
		break;
	case FLOAT:
		t = Token(std::stof(content));
		break;
	case PERCENT:
		t = Token(std::stod(content),true);
		break;
	case UNICODE:
		t = Token(content,true);
		break;
	case SINGLE_CHAR:
		t = Token(type);
		break;
	}

	t.Position.Column = col;
	t.Position.Line = cline;
	return t;
}

void arda::Lexer::AddEol(std::shared_ptr<TokenStream> stream,int col)
{
	bool add = (stream->GetTokens().size()==0) ? true :  stream->GetBack().Type != Token::EndOfLine;
	
	if (add)
	{
		Token t(Token::EndOfLine);
		t.Position.Line = cline;
		t.Position.Column = col;
		stream->AddToken(t);
	}
	++cline;
}

bool arda::Lexer::CheckEol(const std::string & line, std::shared_ptr<TokenStream> stream)
{
	if (line.size() == 0)
		AddEol(stream,0);

	return false;
}

void arda::Lexer::SkipWhitespaces(const std::string & str, int& pos)
{
	while (isspace(str[pos]))
		++pos;
}

void arda::Lexer::Preprocess(const std::string & str, int & pos, std::shared_ptr<ParsingContext> context)
{
	SkipWhitespaces(str, pos);

	std::string cmd = ReadToWs(str, pos);

	SkipWhitespaces(str, pos);

	if (cmd == "#define")
	{
		std::string name = ReadToWs(str, pos);
		SkipWhitespaces(str, pos);
		std::string value = ReadToWs(str, pos);

		context->AddMacro(name, value);
	}
	else
	{
		int a = 0;
	}
}

std::string arda::Lexer::ReadToWs(const std::string & str, int & pos)
{
	std::string word;
	char c = str[pos];
	
	while (c != ' ' && pos < str.size())
	{
		word += c;
		c = str[++pos];
	}
		

	return word;
}
