#include "texture.hpp"
#include "image.hpp"

arda::ITexture::ITexture(int width, int height) :
	m_width(width), m_height(height)
{
}

arda::ITexture::ITexture(Image& img) :
	m_width(img.GetWidth()), m_height(img.GetHeight())
{
}
