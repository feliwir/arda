#include "global.hpp"
#include "debugger.hpp"

arda::Global::Global(Config & c) : m_debugger(std::make_shared<Debugger>(c))
{
	m_start = clock::now();
}
