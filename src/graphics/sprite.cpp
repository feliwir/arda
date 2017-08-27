#include "sprite.hpp"

arda::Sprite::Sprite()
{
}

arda::Sprite::Sprite(std::shared_ptr<Texture> tex) :
	m_texture(tex)
{
}

arda::Sprite::~Sprite()
{
}

void arda::Sprite::Render()
{
}
