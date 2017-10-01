#include "ini_mappedImage.hpp"
#include "../ini.hpp"


arda::ini::MappedImage::MappedImage()
{
}

void arda::ini::MappedImage::SetProperty(std::string_view name, const Property value)
{
	if (name == "Texture")
		m_texture = value.GetString();
	else if (name == "TextureWidth")
	{
		m_texWidth = value.GetInteger();
	}
	else if (name == "TextureHeight")
	{
		m_texHeight = value.GetInteger();
	}
	else if (name == "Coords")
	{
		m_coords = value.GetCoords();
	}
	else if (name == "Status")
	{
		m_status = value.GetString();
	}
	
}

void arda::ini::MappedImage::Register(Ini & ini, std::string_view name)
{
	ini.RegisterMappedImage(shared_from_this(), name);
}
