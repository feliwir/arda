#include "property.hpp"
#include "../../core/debugger.hpp"

arda::Property::Property(std::vector<Token>& args) : m_args(args)
{
}

bool arda::Property::GetBoolean() const
{
	if (!CheckArgs())
		return false;

	bool result = false;

	std::string str = std::get<std::string>(m_args.front().Value);
	
	if (str == "Yes")
		result = true;
	else if (str == "No")
		result = false;
	else
		ARDA_LOG("Can't create Boolean Property: " + str);

	return result;
}

std::string arda::Property::GetString() const
{
	if (!CheckArgs())
		return "";

	auto& token = m_args.front();

	if (token.Type != Token::StringLiteral)
	{
		

		if (token.Type == Token::IntegerLiteral)
		{
			return std::to_string(std::get<long long>(token.Value));
		}
		else
		{
			ARDA_LOG("Cannot create String property!");
			return "";
		}
			
	}
	else
		return std::get<std::string>(token.Value);
}

std::string arda::Property::GetUnicode() const
{
	if (!CheckArgs())
		return "";

	if (m_args.front().Type != Token::UnicodeLiteral)
	{
		ARDA_LOG("Expected Unicode Literal!");
		return "";
	}

	return std::get<std::string>(m_args.front().Value);
}

long long arda::Property::GetInteger() const
{
	if (!CheckArgs())
		return 0;

	auto& token = m_args.front();

	//cast into an int
	if (token.Type == Token::FloatLiteral)
	{
		return static_cast<long long>(std::get<double>(token.Value));
	}

	return std::get<long long>(token.Value);
}

double arda::Property::GetDouble() const
{
	if (!CheckArgs())
		return 0;

	auto& token = m_args.front();

	//cast into a double
	if (token.Type == Token::IntegerLiteral)
	{
		return static_cast<double>(std::get<long long>(token.Value));
	}

	return std::get<double>(token.Value);
}

glm::ivec4 arda::Property::GetCoords() const
{
	if (!CheckArgs())
		return glm::ivec4();
	glm::ivec4 result = glm::ivec4();
	for (int i = 0; i < m_args.size(); i++)
	{
		
		int k = 1;
	}
	return result;
}

bool arda::Property::CheckArgs() const
{
	if (std::empty(m_args))
	{
		ARDA_LOG("Missing arguments for property");
		return false;
	}

	return true;
}
