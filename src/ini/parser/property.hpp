#pragma once
#include <vector>
#include "token.hpp"

namespace arda
{
	class Property
	{
	public:
		Property(std::vector<Token>& args);

		bool GetBoolean() const;
		std::string GetString() const;
		long long GetInteger() const;
	private:
		std::vector<Token> m_args;
	};
}