#include "ini_musicTrack.hpp"
#include "../ini.hpp"

int arda::ini::MusicTrack::m_defaultVolume = 0;

arda::ini::MusicTrack::MusicTrack()
	:m_volume(m_defaultVolume)
{
}

void arda::ini::MusicTrack::SetProperty(std::string_view name, const Property value)
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

void arda::ini::MusicTrack::Register(Ini & ini, std::string_view name)
{
	ini.RegisterMusicTrack(shared_from_this(), name);
}
