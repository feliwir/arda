#include "gui.hpp"

arda::GUI::GUI(Config& c, Graphics& g, FileSystem& fs) :
	m_graphics(g),m_filesystem(fs)
{

}

arda::GUI::~GUI()
{
}

void arda::GUI::Update()
{
}

void arda::GUI::SetActive(std::string_view name)
{
}
