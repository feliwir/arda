#include "lexer.hpp"
#include <sstream>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <chrono>
#include <boost/filesystem.hpp>
#include <iostream>
#include "string_util.hpp"
#include "../core/debugger.hpp"
#include "../ini.hpp"
#include "../../filesystem/stream.hpp"

namespace fs = boost::filesystem;
using namespace std::placeholders;

std::map<std::string, std::string> arda::ParsingContext::m_globalMacros;

std::shared_ptr<arda::ParsingContext> arda::Lexer::Lex(std::shared_ptr<IStream> stream,const std::string& path)
{
	auto context = std::make_shared<ParsingContext>();
	m_path = path;
	m_basepath = path.substr(0, path.find_last_of('/') + 1);
	m_line = 0;

	auto tokens = std::make_shared<TokenStream>(path);
	context->SetTokens(tokens);

	std::string line;
	std::string_view line_view;
	size_t pos;
	std::stringstream sourcestream;
	std::string source = stream->ReadAll();
	sourcestream << source;

	auto start = std::chrono::high_resolution_clock::now();
	auto end = start;

	while (std::getline(sourcestream, line))
	{
		//remove all \r
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

		//replace tabs with spaces
		std::replace(line.begin(), line.end(), '\t', ' ');

		line_view = line;

		if (CheckEol(line_view, tokens))
			continue;
			
		//remove comments
		pos = line_view.find(';');
		if (pos != std::string::npos)
		{
			line_view = line_view.substr(0, pos);
		}

		if (CheckEol(line_view, tokens))
			continue;

		pos = line_view.find("//");
		if (pos != std::string::npos)
		{
			line_view = line_view.substr(0, pos);
		}

		if (CheckEol(line_view, tokens))
			continue;

		//remove trailing whitespaces
		line_view = Trim(line_view);

		if (CheckEol(line_view, tokens))
			continue;


		tokens->InsertTokens(Tokenize(line_view, context));

		AddEol(tokens,pos);
	}

	end = std::chrono::high_resolution_clock::now();
	tokens->AddToken(Token(Token::EndOfFile));

	return context;
}

arda::Lexer::Lexer(Ini & ini, FileSystem & fs) : m_ini(ini), m_fs(fs)
{
}

std::vector<arda::Token> arda::Lexer::Tokenize(std::string_view line, std::shared_ptr<ParsingContext> context,bool tokenstream)
{
	std::vector<Token> tokens;
	int pos = 0;
	Skip<' '>(line, pos);

	while (pos < line.size())
	{
		Token tok = CreateToken(line, pos,tokens, context, tokenstream);
		if(tok.Type!=Token::Unknown)
			tokens.push_back(tok);

		Skip<' '>(line, pos);
	}

	return tokens;
}

arda::Token arda::Lexer::CreateToken(std::string_view line, int & pos, std::vector<Token>& toks, std::shared_ptr<ParsingContext> context,bool tokenstream)
{
	bool parse = true;
	Token t(Token::Unknown);
	LexerMode mode = NORMAL;
	Token::TokenType type = Token::Unknown;
	std::string content;
	int col = pos;
	char c = 0;

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

			//we started with an int, that now turns out as a float
			if (mode == INTEGER && c == '.')
			{
				mode = FLOAT;
			}

			//we started with an int, that now turns out as a string
			if (mode == INTEGER && (isalpha(c) || c == '_'))
			{
				mode = STRING;
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
			if (tokenstream)
			{
				auto macro_toks = Tokenize(content, context);
				toks.insert(toks.end(), macro_toks.begin(), macro_toks.end());
			}
			else
			{
				std::vector<Token> toks = Tokenize(content, context, false);
				MergeTokens(toks, 1);
				t = toks.front();
			}
				
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

bool arda::Lexer::CheckEol(std::string_view line, std::shared_ptr<TokenStream> stream)
{
	if (line.size() == 0)
	{
		AddEol(stream, 0);
		return true;
	}
		
	return false;
}

void arda::Lexer::SkipWhitespaces(std::string_view str, int& pos)
{
	while (isspace(str[pos]))
		++pos;
}

void arda::Lexer::Preprocess(std::string_view str, int & pos, std::shared_ptr<ParsingContext> context)
{
	Skip<' '>(str, pos);

	std::string_view cmd = ReadCmd(str, pos);

	Skip<' '>(str, pos);

	if (cmd == "#define")
	{
		std::string_view name = ReadTill<' '>(str, pos);
		if (name == "TUTORIAL_WOTR_VOLUME")
		{
			int a = 0;
		}

		Skip<' '>(str, pos);
		std::string_view value = str.substr(pos, str.size());
		pos = str.size();
		context->AddMacro(std::string(name), std::string(value));
	}
	else if (cmd=="#include")
	{
		std::string_view include = ReadQuoted(str, pos);
		std::string inc_path = m_basepath + std::string(include);
		inc_path = fs::weakly_canonical(fs::path(inc_path)).string();
		std::replace(inc_path.begin(), inc_path.end(), '\\', '/');
		std::transform(inc_path.begin(), inc_path.end(), inc_path.begin(), ::tolower);
		context->Include(m_ini.GetContext(inc_path));
	}
	else
	{
		std::function<Token(Token&, Token&)> op;
		bool known = true;
		
		if (cmd == "#MULTIPLY")
			op = std::bind(&Lexer::TokenOperation, this, _1, _2, [](const double a, const double b) {return a*b; });
		else if (cmd == "#ADD")
			op = std::bind(&Lexer::TokenOperation, this, _1, _2, [](const double a, const double b) {return a+b; });
		else if (cmd == "#DIVIDE")
			op = std::bind(&Lexer::TokenOperation, this, _1, _2, [](const double a, const double b) {return a/b; });
		else if (cmd == "#SUBTRACT")
			op = std::bind(&Lexer::TokenOperation, this, _1, _2, [](const double a, const double b) {return a-b; });
		else
		{
			known = false;
			//std::cout << "UNKNOWN PREPROCESSOR FUNCTION: " << cmd << std::endl;
		}
			
		if(known)
			TokenFunc(op, str, pos, context);
	}
}

void arda::Lexer::MergeTokens(std::vector<Token>& tokens, int max)
{
	if (tokens.size() > max)
	{
		auto it = std::begin(tokens);
		while (it != std::end(tokens)) {
			// Do some stuff
			if (it->Type == Token::Minus)
			{
				auto after = std::next(it);
				if (after->Type == Token::IntegerLiteral)
					after->Value = -std::get<long long>(after->Value);
				else if (after->Type == Token::FloatLiteral)
					after->Value = -std::get<double>(after->Value);

				it = tokens.erase(it);
			}
			else if (it->Type == Token::Plus)
			{
				auto after = std::next(it);
				if (after->Type == Token::IntegerLiteral)
					after->Value = +std::get<long long>(after->Value);
				else if (after->Type == Token::FloatLiteral)
					after->Value = +std::get<double>(after->Value);

				it = tokens.erase(it);
			}

			std::advance(it, 1);
		}
	}
}

void arda::Lexer::TokenFunc(std::function<Token(Token&, Token&)> func, std::string_view str, int pos, std::shared_ptr<ParsingContext> context)
{
	Token tok1, tok2;
	std::string_view content = str.substr(pos + 1, str.find_first_of(')')-(pos+1));
	std::vector<Token> toks = Tokenize(content, context, false);

	MergeTokens(toks, 2);

	context->GetTokenStream()->AddToken(func(toks[0], toks[1]));
}

arda::Token arda::Lexer::TokenOperation(const Token & a, const Token & b, std::function<double(double, double)> f)
{
	bool integerTok = ((a.Type == Token::IntegerLiteral) && (b.Type == Token::IntegerLiteral)) ? true : false;

	double first = (a.Type == Token::IntegerLiteral) ?
		std::get<long long>(a.Value) :
		std::get<double>(a.Value);

	double second = (b.Type == Token::IntegerLiteral) ?
		std::get<long long>(b.Value) :
		std::get<double>(b.Value);

	double result = f(first, second);
	return (integerTok) ? static_cast<long long>(result) : result;
}

std::string_view arda::Lexer::ReadQuoted(std::string_view str, int & pos)
{
	while (str[pos] != '\"' && pos < str.size()) ++pos;
		++pos;

	int first = pos;

	while (str[pos] != '\"' && pos < str.size())
		++pos;
	
	int last = pos;
	
	return str.substr(first, last - first);
}

std::string_view arda::Lexer::ReadCmd(std::string_view  str, int & pos)
{
	char c = str[pos];
	int first = pos;

	while (c != ' '&&c!='(' && pos < str.size())
		c = str[++pos];
	
	int last = pos;
	return str.substr(first,last-first);
}