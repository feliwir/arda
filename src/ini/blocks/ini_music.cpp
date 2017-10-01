#include "ini_music.hpp"
#include "../ini.hpp"

int arda::ini::Music::m_defaultVolume = 0;

arda::ini::Music::Music()
	:m_volume(m_defaultVolume)
{
}

void arda::ini::Music::SetProperty(std::string_view name, const Property value)
{
	if (name == "Filename")
		m_filename = value.GetString();
	else if (name == "Volume")
	{
		m_volume = value.GetInteger();
	}
	else if (name == "SubmixSlider")
	{
		m_submixSlider = value.GetString();
	}
	else if (name == "ReverbEffektLevel")
	{
		m_reverbEffektLevel = value.GetInteger();
	}
	else if (name == "DryLevel")
	{
		m_dryLevel = value.GetInteger();
	}
	else if (name == "Control")
	{
		m_control = value.GetString();
	}
	else if (name == "Type")
	{
		m_type = value.GetString();
	}
}

void arda::ini::Music::Register(Ini & ini, std::string_view name)
{
	ini.RegisterMusic(shared_from_this(), name);
}
