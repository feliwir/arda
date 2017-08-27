#pragma once
#include <vector>
#include <map>
#include "token.hpp"

namespace arda
{
	class TokenStream
	{
	public:
		inline TokenStream(const std::string& file)
		{
			m_file = file;
			m_currentToken = 0;
		}

		inline void AddToken(const Token& t)
		{
			m_tokens.push_back(t);
		}

		inline void InsertTokens(const std::vector<Token>& tokens)
		{
			m_tokens.insert(m_tokens.end(), tokens.begin(), tokens.end());
		}

		inline const Token& GetToken()
		{
			return m_tokens[m_currentToken++];
		}

		inline std::vector<Token>& GetTokens()
		{
			return m_tokens;
		}

		inline void Start()
		{
			m_currentToken = 0;
		}

		inline const Token& Current()
		{
			return m_tokens[m_currentToken];
		}

		inline const Token& Peek()
		{
			return m_tokens[m_currentToken + 1];
		}

		inline const Token GetBack()
		{
			return m_tokens.back();
		}

		inline void Clear()
		{
			m_tokens.clear();
		}
	private:
		std::string m_file;
		int m_currentToken;
		std::vector<Token> m_tokens;
	};
}