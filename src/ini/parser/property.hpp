#pragma once
#include <vector>
#include "token.hpp"
#include <glm/glm.hpp>

namespace arda
{
	class Property
	{
	public:
		Property(std::vector<Token>& args);

		bool GetBoolean() const;
		std::string GetString() const;
		std::string GetUnicode() const;
		long long GetInteger() const;
		double GetDouble() const;
		glm::ivec4 GetCoords() const;

		bool CheckArgs() const;
	private:
		std::vector<Token> m_args;
	};
}