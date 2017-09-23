#include "property.hpp"
#include "../../core/debugger.hpp"

arda::Property::Property(std::vector<Token>& args) : m_args(args)
{
}

bool arda::Property::GetBoolean() const
{
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
	if (m_args.front().Type != Token::StringLiteral)
	{
		ARDA_LOG("Expected String Literal!");
		return "";
	}

	return std::get<std::string>(m_args.front().Value);
}

long long arda::Property::GetInteger() const
{
	return std::get<long long>(m_args.front().Value);
}
