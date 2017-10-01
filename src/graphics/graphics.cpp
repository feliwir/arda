#include "graphics.hpp"
#include "sprite.hpp"
#include "image.hpp"
#include "gl/gl_renderer.hpp"
#include "d3d/d3d_renderer.hpp"
#include "../core/config.hpp"
#include "../core/debugger.hpp"
#include "../filesystem/filesystem.hpp"
#include <GLFW/glfw3.h>

void glfw_error(int error, const char* description)
{
	ARDA_LOG(description);
}

arda::Graphics::Graphics(Config& c) :
	m_artFolder("art/")
	, m_compiledFolder("compiledtextures/")
	, m_regularFolder("textures/")
	, m_texFolders({ m_compiledFolder,m_regularFolder })
	, m_texExtensions({ ".dds",".tga",".jpg" })
{
	//Initialize glfw
	glfwInit();

	glfwSetErrorCallback(glfw_error);

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

		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(window, NULL, 0, 0, 800, 600, 0);
	}
}

void arda::Graphics::CenterWindow()
{
	auto window = m_renderer->GetWindow();
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	//get window size
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	//now calculate the position

	int posx = mode->width / 2 - width / 2;
	int posy = mode->height / 2 - height / 2;

	glfwSetWindowPos(window, posx, posy);
}

void arda::Graphics::HideCursor()
{
	GLFWwindow* window = m_renderer->GetWindow();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void arda::Graphics::ShowCursor()
{
	GLFWwindow* window = m_renderer->GetWindow();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

std::shared_ptr<arda::Sprite> arda::Graphics::CreateSprite(std::shared_ptr<ITexture> tex)
{
	return std::make_shared<arda::Sprite>(*m_renderer, tex);
}

std::shared_ptr<arda::ITexture> arda::Graphics::GetTexture(std::string_view name,FileSystem& fs)
{
	auto str_name = std::string(name);
	auto& it = m_textures.find(str_name);

	if (it != m_textures.end())
		return it->second;


	std::shared_ptr<ITexture> result;

	//first try with compiled texture
	for (auto& folder : m_texFolders)
	{
		for (auto& extension : m_texExtensions)
		{
			std::string path = m_artFolder + folder + str_name.substr(0, 2)+'/' + str_name + extension;
			auto stream = fs.GetStream(path);

			if (stream == nullptr)
				continue;

			Image img(stream);
			result = m_renderer->CreateTexture(img);
		}
	}

	return result;
}

std::shared_ptr<arda::Sprite> arda::Graphics::GetMappedImage(std::string_view name, FileSystem & fs, Ini & ini)
{
	return std::shared_ptr<Sprite>();
}

void arda::Graphics::ClearTextures()
{
}
