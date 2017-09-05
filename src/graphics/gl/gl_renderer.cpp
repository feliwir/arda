#include "gl_renderer.hpp"
#include "gl_texture.hpp"

arda::GLRenderer::GLRenderer(Config& c) : IRenderer(c)
{
	flextInit();

	//register debug callback
	if (FLEXT_KHR_debug)
	{
		
		glDebugMessageCallback(DebugCallback, nullptr);
	}
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

void arda::GLRenderer::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam)
{
	int a = 0;
}
