#include "gl_renderer.hpp"
#include "gl_texture.hpp"
#include "flextGL.h"

arda::GLRenderer::GLRenderer()
{
	flextInit();
}

void arda::GLRenderer::Render()
{
}

std::shared_ptr<arda::ITexture> arda::GLRenderer::CreateTexture()
{
	return std::make_shared<GLTexture>(0,0);
}

std::shared_ptr<arda::ITexture> arda::GLRenderer::CreateTexture(Image & img)
{
	return std::make_shared<GLTexture>(img);
}
