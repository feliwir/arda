#pragma once
#include <memory>
#include <vector>
#include <map>
#include "tokenstream.hpp"

namespace arda
{
	class ParsingContext
	{
	public:
		ParsingContext() {}
		ParsingContext(const ParsingContext& that) = delete;

		inline void SetTokens(std::shared_ptr<TokenStream> tokens)
		{
			m_tokens = tokens;
		}

		inline std::shared_ptr<TokenStream> GetTokens()
		{
			return m_tokens;
		}

		inline std::map<std::string, std::string>& GetMacros()
		{
			return m_macros;
		}

		inline void AddMacro(const std::string& name, const std::string& value)
		{
			m_macros[name] = value;
		}

		std::vector<std::shared_ptr<ParsingContext>>& GetIncludes()
		{
			return m_includes;
		}
	private:
		std::map<std::string, std::string> m_macros;
		std::shared_ptr<TokenStream> m_tokens;
		std::vector<std::shared_ptr<ParsingContext>> m_includes;
	};
}