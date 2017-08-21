#pragma once
#include <string>
#include <memory>
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
		static std::shared_ptr<ParsingContext> Lex(std::shared_ptr<IStream> stream, 
												const std::string& filename, 
												Ini& ini,FileSystem& fs);
	private:
		static Token CreateToken(const std::string& line, int& pos, std::shared_ptr<ParsingContext>);
		static void AddEol(std::shared_ptr<TokenStream> stream,int col);
		static bool CheckEol(const std::string& line, std::shared_ptr<TokenStream> stream);
		static void SkipWhitespaces(const std::string& str, int& pos);
		static void Preprocess(const std::string& str, int& pos,std::shared_ptr<ParsingContext> context);
		static std::string ReadToWs(const std::string& str, int& pos);


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

		static int cline;
	};
}