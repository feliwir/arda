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

		inline std::shared_ptr<TokenStream> GetTokenStream()
		{
			return m_tokens;
		}

		inline std::map<std::string, std::string>& GetMacros()
		{
			return m_macros;
		}

		static std::map<std::string, std::string>& GetGlobalMacros()
		{
			return m_globalMacros;
		}

		inline void AddMacro(const std::string& name, const std::string& value)
		{
			m_macros[name] = value;
		}

		inline const bool CheckMacro(std::string& value)
		{
			auto it = m_macros.find(value);
			if (it != m_macros.end())
			{
				value = it->second;
				return true;
			}		
			else
			{
				it = m_globalMacros.find(value);
				if (it != m_globalMacros.end())
				{
					value = it->second;
					return true;
				}
				else
					return false;
			}
				
		}

		inline void Include(std::shared_ptr<ParsingContext> include)
		{
			if(include)
				m_macros.insert(include->GetMacros().begin(), include->GetMacros().end());
		}


	private:
		std::map<std::string, std::string> m_macros;
		static std::map<std::string, std::string> m_globalMacros;
		std::shared_ptr<TokenStream> m_tokens;
	};
}