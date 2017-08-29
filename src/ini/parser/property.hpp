#pragma once
#include <vector>
#include "token.hpp"

namespace arda
{
	class Property
	{
	public:
		Property(std::vector<Token>& args);

		std::string GetString();
		long long GetInteger();
	private:
		std::vector<Token> m_args;
	};
}