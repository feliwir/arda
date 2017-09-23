#include "graphics.hpp"
#include "sprite.hpp"
#include "gl/gl_renderer.hpp"
#include "d3d/d3d_renderer.hpp"
#include "../core/config.hpp"
#include "../core/debugger.hpp"
#include <GLFW/glfw3.h>

arda::Graphics::Graphics(Config& c)
{
	//Initialize glfw
	glfwInit();

	m_clearColor = { 0.0,0.2,0.0,1.0 };

	switch (c.GetGraphicsAPI())
	{
	//OPENGL
	default:
	case 0:
		ARDA_LOG("Initialize OpenGL renderer!");
		m_renderer = std::make_unique<GLRenderer>(c);
		break;
	//DIRECT3D 11
	case 1:
		#if ARDA_USE_D3D
		ARDA_LOG("Initialize Direct3D 11 renderer!");
		m_renderer = std::make_unique<D3DRenderer>(c);
		#else
		ARDA_LOG("Direct3D 11 is not available on this platform!");
		#endif
		break;
	}
	

	m_renderer->SetClearColor(m_clearColor);
}

void arda::Graphics::Clear()
{
	m_renderer->Clear();
}

void arda::Graphics::Render()
{
	m_renderer->Render();	
}

void arda::Graphics::Present()
{
	m_renderer->Present();
}

void arda::Graphics::SetFullscreen(const bool full)
{
	GLFWwindow* window = m_renderer->GetWindow();
	GLFWmonitor* monitor = glfwGetWindowMonitor(window);
	bool isFullscreen = (monitor != nullptr);

	if (full == isFullscreen)
	{
		ARDA_LOG("Redundant SetFullscreen call!");
		return;
	}

	if (full)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		
		glfwSetWindowMonitor(window,monitor,0,0, mode->width, mode->height,mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(window, NULL, 0, 0, 800, 600, 0);
	}
}

std::shared_ptr<arda::Sprite> arda::Graphics::CreateSprite(std::shared_ptr<ITexture> tex)
{
	return std::make_shared<arda::Sprite>(*m_renderer,tex);
}
