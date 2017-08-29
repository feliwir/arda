#include "property.hpp"

arda::Property::Property(std::vector<Token>& args) : m_args(args)
{
}

std::string arda::Property::GetString()
{
	return std::get<std::string>(m_args.front().Value);
}

long long arda::Property::GetInteger()
{
	return std::get<long long>(m_args.front().Value);
}
