#pragma once
#include <vector>
#include <map>
#include "token.hpp"

namespace arda
{
	class TokenStream
	{
	public:
		TokenStream()
		{
			int a = 0;
		}

		inline TokenStream(const std::string& file)
		{
			m_file = file;
			m_currentToken = 0;
		}

		inline void AddToken(const Token& t)
		{
			m_tokens.push_back(t);
		}

		inline const Token GetToken()
		{
			return m_tokens[m_currentToken];
			++m_currentToken;
		}

		//Preprocessor related
		inline const std::string& GetMacro(const std::string& macro)
		{
			return m_macros[macro];
		}

		inline std::map<std::string, std::string>& GetMacros()
		{
			return m_macros;
		}

		inline void AddMacro(const std::string& macro,const std::string& value)
		{
			m_macros[macro] = value;
		}
	private:
		std::map<std::string, std::string> m_macros;
		std::string m_file;
		int m_currentToken;
		std::vector<Token> m_tokens;
	};
}