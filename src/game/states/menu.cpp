#include "menu.hpp"
#include "../../gui/gui.hpp"

arda::Menu::Menu(GUI& gui) : m_gui(gui)
{
	m_gui.SetActive("MainMenu");
}

bool arda::Menu::IsFinished()
{
    return false;
}

void arda::Menu::Start()
{
}

void arda::Menu::Update()
{
}