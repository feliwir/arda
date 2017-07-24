#include "graphics.hpp"
#include "gl/gl_renderer.hpp"

arda::Graphics::Graphics(Config& c)
{
	m_renderer = std::make_unique<GLRenderer>();


}