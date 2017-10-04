#include "apt.hpp"

arda::Apt::Apt(std::shared_ptr<IStream> stream, std::shared_ptr<Constant> data) : 
	m_stream(stream)
{
}

arda::Apt::~Apt()
{
}
