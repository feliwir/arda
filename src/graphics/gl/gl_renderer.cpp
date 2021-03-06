#include "gl_renderer.hpp"
#include "gl_texture.hpp"
#include "gl_shader.hpp"
#include "gl_shadercode.hpp"
#include "gl_buffer.hpp"
#include "gl_layout.hpp"
#include "../../core/debugger.hpp"
#include "../../core/config.hpp"
#include <GLFW/glfw3.h>

arda::GLRenderer::GLRenderer(Config& c) : IRenderer(c)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

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
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugCallback, nullptr);
	}

	Resize(c.GetWidth(), c.GetHeight());

	RegisterCallbacks();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_spriteShader = std::make_unique<GLShader>();
	m_spriteShader->Compile(shader::sprite_vert, shader::sprite_frag);
	m_spriteShader->Link();
	m_spriteShader->AddUniform("color_tex");
	m_spriteShader->AddUniform("mask_tex");
	m_spriteShader->AddUniform("opacity");
	m_spriteShader->AddUniform("use_mask");
}

void arda::GLRenderer::Render()
{
	ActivateShader(m_spriteShader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	GetActiveShader()->SetIntegerProperty("color_tex", 0);
	GetActiveShader()->SetIntegerProperty("mask_tex", 1);

	//sort and bind the correct shader
	for (auto& drawable : m_drawables)
	{
		drawable->Render(*this);
	}
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

std::shared_ptr<arda::Layout> arda::GLRenderer::CreateLayout()
{
	return std::make_shared<GLLayout>();
}

std::shared_ptr<arda::IBuffer> arda::GLRenderer::CreateBuffer(IBuffer::Type t, IBuffer::Usage u)
{
	return std::make_shared<GLBuffer>(t,u);
}

void arda::GLRenderer::SetClearColor(const glm::vec4 & color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void arda::GLRenderer::Resize(const int width, const int height)
{
	glViewport(0, 0, width, height);
}

void arda::GLRenderer::Draw(std::shared_ptr<IBuffer> vertices, std::shared_ptr<IBuffer> indices)
{
	vertices->Bind();
	indices->Bind();
	glDrawElements(GL_TRIANGLES, indices->GetElementNumber(), GL_UNSIGNED_SHORT,NULL);
}

void arda::GLRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
