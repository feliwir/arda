#include "application.hpp"
#include <GLFW/glfw3.h>
#include "config.hpp"
#include "global.hpp"
#include "../filesystem/filesystem.hpp"
#include "../filesystem/stream.hpp"
#include "../ini/tokenizer.hpp"
std::unique_ptr<arda::Global> arda::Application::s_global;

arda::Application::Application(const std::vector<std::string>& args)
	: m_window(nullptr)
{
	//Initialize global variables
	s_global = std::make_unique<Global>();

	//Load configuration first
	m_config = std::make_unique<Config>(args);

	//Initialize virtual filesystem
	m_fs = std::make_unique<FileSystem>(*m_config);

	auto map = m_fs->listDirectory("data/ini");
	auto weapon = m_fs->getFile("data/ini/weapon.ini");
	std::string buffer(weapon->getSize(), ' ');
	weapon->read(&buffer[0], buffer.size());
	Tokenizer tok;
	tok.Parse(buffer);

	glfwInit();


	//Initialize ini system

	m_window = glfwCreateWindow(m_config->getWidth(), 
								m_config->getHeight(), 
								m_config->getTitle().c_str(),
								0, 0);


	glfwShowWindow(m_window);
}

arda::Application::~Application()
{
	if (m_window)
		glfwDestroyWindow(m_window);
}

void arda::Application::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

}
