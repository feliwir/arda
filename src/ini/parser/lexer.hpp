#pragma once
#include <string>
#include <string_view>
#include <memory>
#include <functional>
#include "context.hpp"

namespace arda
{
	class Ini;
	class FileSystem;
	class IStream;

	class Lexer
	{
	private:
		enum LexerMode
		{
			NORMAL,
			INTEGER,
			FLOAT,
			STRING,
			UNICODE,
			PERCENT,
			SINGLE_CHAR,
		};
	public:
		Lexer(Ini& ini, FileSystem& fs);

		std::shared_ptr<ParsingContext> Lex(std::shared_ptr<IStream> stream, 
												const std::string& filename);
	private:
		std::vector<Token> Tokenize(std::string_view, std::shared_ptr<ParsingContext> context = nullptr,bool tokenstream=true);
		Token CreateToken(std::string_view line, int& pos, std::vector<Token>& toks,std::shared_ptr<ParsingContext> context=nullptr,bool tokenstream=true);
		void AddEol(std::shared_ptr<TokenStream> stream,int col);
		bool CheckEol(std::string_view line, std::shared_ptr<TokenStream> stream);
		void SkipWhitespaces(std::string_view str, int& pos);
		void Preprocess(std::string_view str, int& pos,std::shared_ptr<ParsingContext> context);
		
		std::string_view ReadQuoted(std::string_view str, int & pos);
		std::string_view ReadCmd(std::string_view str, int& pos);

		void MergeTokens(std::vector<Token>& tokens, int max);

		void TokenFunc(std::function<Token(Token&, Token&)> func, std::string_view str, int pos, std::shared_ptr<ParsingContext> context);

		Token TokenOperation(const Token & a, const Token & b, std::function<double(const double, const double)> f);
	private:
		Ini& m_ini;
		FileSystem& m_fs;
		std::string m_basepath;
		std::string m_path;
		int m_line;
	};
}