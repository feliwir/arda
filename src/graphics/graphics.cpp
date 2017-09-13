#include "graphics.hpp"
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
		m_renderer = std::make_unique<GLRenderer>(c);
		break;
	//DIRECT3D 11
	case 1:
		#if ARDA_USE_D3D
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

	
}

void arda::Graphics::Present()
{
	m_renderer->Present();
}
