#include "ini_video.hpp"
#include "../ini.hpp"

int arda::ini::Video::m_defaultVolume = 0;

arda::ini::Video::Video() : 
	m_isDefault(false),
	m_volume(m_defaultVolume)
{
}

void arda::ini::Video::SetProperty(std::string_view name, const Property value)
{
	if (name == "Filename")
		m_filename = value.GetString();
	else if (name == "Volume")
	{
		m_volume = value.GetInteger();

		if (m_isDefault)
			m_defaultVolume = m_volume;
	}
	else if (name == "Comment")
		m_comment = value.GetUnicode();	
	else if (name == "IsDefault")
	{
		m_isDefault = value.GetBoolean();

		if (m_isDefault)
			m_defaultVolume = m_volume;
	}
}

void arda::ini::Video::Register(Ini & ini, std::string_view name)
{
	ini.RegisterVideo(shared_from_this(),name);
}
