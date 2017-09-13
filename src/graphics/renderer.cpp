#include "renderer.hpp"
#include "../core/config.hpp"
#include "../core/debugger.hpp"
#include <GLFW/glfw3.h>

arda::IRenderer::IRenderer(Config& c) : m_window(nullptr)
{
	
}

arda::IRenderer::~IRenderer()
{
	if (m_window)
		glfwDestroyWindow(m_window);
}

void arda::IRenderer::RegisterCallbacks()
{
	glfwSetWindowSizeLimits(m_window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwSetWindowUserPointer(m_window, this);

	glfwSetFramebufferSizeCallback(m_window, CallbackResize);
}

void arda::IRenderer::CallbackResize(GLFWwindow * window, int width, int height)
{
	//get the attached IRenderer
	IRenderer* renderer = static_cast<arda::IRenderer*>(glfwGetWindowUserPointer(window));

	ARDA_LOG("Resize: W-" + std::to_string(width) + " H-" + std::to_string(height));

	renderer->Resize(width, height);
}
