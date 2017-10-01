#include "mappedimage.hpp"

arda::MappedImage::MappedImage(std::shared_ptr<ITexture> tex, std::vector<glm::vec2>& coords, std::shared_ptr<ITexture> mask) 
	: m_texture(tex), m_coords(coords), m_mask(mask)
{
}
