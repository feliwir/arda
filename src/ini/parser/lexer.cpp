#include "lexer.hpp"
#include <sstream>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <chrono>
#include <boost/filesystem.hpp>
#include <iostream>
#include "../ini.hpp"
#include "../../filesystem/stream.hpp"
namespace fs = boost::filesystem;

std::map<std::string, std::string> arda::ParsingContext::m_globalMacros;

std::shared_ptr<arda::ParsingContext> arda::Lexer::Lex(std::shared_ptr<IStream> stream, const std::string & path)
{
	auto context = std::make_shared<ParsingContext>();
	m_path = path;
	m_basepath = path.substr(0, path.find_last_of('/') + 1);
	m_line = 0;

	auto tokens = std::make_shared<TokenStream>(path);
	context->SetTokens(tokens);

	std::string line;
	int pos;
	std::stringstream sourcestream;
	std::string source = stream->readAll();
	sourcestream << source;

	//auto start = std::chrono::high_resolution_clock::now();
	//auto end = start;

	while (std::getline(sourcestream, line))
	{/*
		start = std::chrono::high_resolution_clock::now();*/

		//remove all \r
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

		//replace tabs with spaces
		std::replace(line.begin(), line.end(), '\t', ' ');

		if (CheckEol(line, tokens))
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



		tokens->InsertTokens(Tokenize(line, context));


		AddEol(tokens,pos);

		/*end = std::chrono::high_resolution_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() <<"ns" << std::endl;*/
	}

	tokens->AddToken(Token(Token::EndOfFile));

	return context;
}

arda::Lexer::Lexer(Ini & ini, FileSystem & fs) : m_ini(ini), m_fs(fs)
{
}

std::vector<arda::Token> arda::Lexer::Tokenize(const std::string & line, std::shared_ptr<ParsingContext> context)
{
	std::vector<Token> tokens;
	int pos = 0;
	while (pos < line.size())
	{
		Token tok = CreateToken(line, pos, context);
		if(tok.Type!=Token::Unknown)
			tokens.push_back(tok);
	}

	return tokens;
}

arda::Token arda::Lexer::CreateToken(const std::string & line, int & pos, std::shared_ptr<ParsingContext> context)
{
	bool parse = true;
	Token t(Token::Unknown);
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
		if (context && context->CheckMacro(content))
			context->GetTokenStream()->InsertTokens(Tokenize(content,context));
		else
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
	t.Position.Line = m_line;
	return t;
}

void arda::Lexer::AddEol(std::shared_ptr<TokenStream> stream,int col)
{
	bool add = (stream->GetTokens().size()==0) ? true :  stream->GetBack().Type != Token::EndOfLine;
	
	if (add)
	{
		Token t(Token::EndOfLine);
		t.Position.Line = m_line;
		t.Position.Column = col;
		stream->AddToken(t);
	}
	++m_line;
}

bool arda::Lexer::CheckEol(const std::string & line, std::shared_ptr<TokenStream> stream)
{
	if (line.size() == 0)
	{
		AddEol(stream, 0);
		return true;
	}
		
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

	std::string cmd = ReadCmd(str, pos);

	SkipWhitespaces(str, pos);

	if (cmd == "#define")
	{
		std::string name = ReadToWs(str, pos);
		SkipWhitespaces(str, pos);
		std::string value = str.substr(pos, str.size());
		pos = str.size();
		context->AddMacro(name, value);
	}
	else if (cmd=="#include")
	{
		std::string include = ReadQuoted(str, pos);
		std::string inc_path = m_basepath + include;
		inc_path = fs::weakly_canonical(fs::path(inc_path)).string();
		std::replace(inc_path.begin(), inc_path.end(), '\\', '/');
		std::transform(inc_path.begin(), inc_path.end(), inc_path.begin(), ::tolower);
		context->Include(m_ini.GetContext(inc_path));
	}
	else if (cmd == "#MULTIPLY")
	{
		int tok_pos= 0;
		Token tok1, tok2;
		++pos;
		SkipWhitespaces(str, pos);

		std::string first = ReadToWs(str, pos);
		context->CheckMacro(first);
		tok1 = CreateToken(first, tok_pos);

		SkipWhitespaces(str, pos);
		std::string second = ReadToWs(str, pos);
		context->CheckMacro(second);
		tok_pos = 0;
		tok2 = CreateToken(second, tok_pos);
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

std::string arda::Lexer::ReadQuoted(const std::string & str, int & pos)
{
	std::string out;

	while (str[pos] != '\"' && pos < str.size()) ++pos;

	++pos;

	while (str[pos] != '\"' && pos < str.size())
	{
		out += str[pos];
		++pos;
	}

	return out;
}

std::string arda::Lexer::ReadCmd(const std::string & str, int & pos)
{
	std::string word;
	char c = str[pos];

	while (c != ' '&&c!='(' && pos < str.size())
	{
		word += c;
		c = str[++pos];
	}


	return word;
}
