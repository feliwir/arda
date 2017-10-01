#include "ini_dialogEvent.hpp"
#include "../ini.hpp"

int arda::ini::DialogEvent::m_defaultVolume = 0;

arda::ini::DialogEvent::DialogEvent() :
	m_volume(m_defaultVolume)
{
}

void arda::ini::DialogEvent::SetProperty(std::string_view name, const Property value)
{
	if (name == "Filename")
		m_filename = value.GetString();
	else if (name == "Volume")
	{
		m_volume = value.GetInteger();
	}
	else if (name == "Comment")
		m_comment = value.GetUnicode();
	else if (name == "SubmixSlider")
	{
		m_submixSlider = value.GetString();
	}
}

void arda::ini::DialogEvent::Register(Ini & ini, std::string_view name)
{
	ini.RegisterDialogEvent(shared_from_this(), name);
}
