#include "gl_renderer.hpp"
#include "gl_texture.hpp"
#include "../../core/debugger.hpp"
#include "../../core/config.hpp"
#include <GLFW/glfw3.h>

arda::GLRenderer::GLRenderer(Config& c) : IRenderer(c)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (c.IsDebug())
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	m_window = glfwCreateWindow(c.GetWidth(),
		c.GetHeight(),
		c.GetTitle().c_str(),
		0, 0);

	glfwMakeContextCurrent(m_window);

	flextInit();

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MAJOR_VERSION, &minor);

	ARDA_LOG("OpenGL version supported: " + std::to_string(major) + "." + std::to_string(minor));

	//register debug callback
	if (FLEXT_KHR_debug)
	{
		glDebugMessageCallback(DebugCallback, nullptr);
	}

	Resize(c.GetWidth(), c.GetHeight());

	RegisterCallbacks();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
}

void arda::GLRenderer::Render()
{
}

void arda::GLRenderer::Present()
{
	glfwSwapBuffers(m_window);
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
	ARDA_LOG(message);
}

void arda::GLRenderer::SetClearColor(const glm::vec4 & color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void arda::GLRenderer::Resize(const int width, const int height)
{
	glViewport(0, 0, width, height);
}

void arda::GLRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
