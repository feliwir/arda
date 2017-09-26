#include "ini_speech.hpp"
#include "../ini.hpp"

int arda::ini::Speech::m_defaultVolume = 0;

arda::ini::Speech::Speech() :
	m_volume(m_defaultVolume)
{
}

void arda::ini::Speech::SetProperty(std::string_view name, const Property value)
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

void arda::ini::Speech::Register(Ini & ini, std::string_view name)
{
	ini.RegisterSpeech(shared_from_this(), name);
}
