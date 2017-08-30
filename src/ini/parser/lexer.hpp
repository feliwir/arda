#pragma once
#include <string>
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
		std::vector<Token> Tokenize(const std::string&, std::shared_ptr<ParsingContext> context = nullptr,bool tokenstream=true);
		Token CreateToken(const std::string& line, int& pos, std::shared_ptr<ParsingContext> context=nullptr,bool tokenstream=true);
		void AddEol(std::shared_ptr<TokenStream> stream,int col);
		bool CheckEol(const std::string& line, std::shared_ptr<TokenStream> stream);
		void SkipWhitespaces(const std::string& str, int& pos);
		void Preprocess(const std::string& str, int& pos,std::shared_ptr<ParsingContext> context);
		

		std::string ReadToWs(const std::string& str, int& pos);
		std::string ReadQuoted(const std::string& str, int & pos);
		std::string ReadCmd(const std::string& str, int& pos);

		static inline std::string trim(const std::string& str)
		{
			size_t first = str.find_first_not_of(' ');
			if (std::string::npos == first)
			{
				return str;
			}
			size_t last = str.find_last_not_of(' ');
			return str.substr(first, (last - first + 1));
		}

		void MergeTokens(std::vector<Token>& tokens, int max);

		void TokenFunc(std::function<Token(Token&, Token&)> func, const std::string& str, int pos, std::shared_ptr<ParsingContext> context);

		Token TokenOperation(const Token & a, const Token & b, std::function<double(const double, const double)> f);
	private:
		Ini& m_ini;
		FileSystem& m_fs;
		std::string m_basepath;
		std::string m_path;
		int m_line;
	};
}