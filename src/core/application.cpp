#include "application.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include "config.hpp"
#include "global.hpp"
#include "../audio/audio.hpp"
#include "../video/video.hpp"
#include "../graphics/graphics.hpp"
#include "../filesystem/filesystem.hpp"
#include "../filesystem/stream.hpp"
#include "../ini/ini.hpp"

std::unique_ptr<arda::Global> arda::Application::s_global;

arda::Application::Application(const std::vector<std::string>& args)
	: m_window(nullptr)
{
	//Initialize global variables
	s_global = std::make_unique<Global>();

	//Load configuration first
	m_config = std::make_unique<Config>(args);

	auto t1 = std::chrono::high_resolution_clock::now();

	//Initialize audio
	m_audio = std::make_unique<Audio>(*m_config);

	//Initialize window
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_config->getWidth(),
		m_config->getHeight(),
		m_config->getTitle().c_str(),
		0, 0);

	glfwMakeContextCurrent(m_window);

	//Initialize graphics
	m_graphics = std::make_unique<Graphics>(*m_config);

	//Initialize virtual filesystem
	m_fs = std::make_unique<FileSystem>(*m_config);

	auto stream = m_fs->getStream("GermanSplash.jpg");
	Image img(stream);

	stream = m_fs->getStream("data/movies/Credits_with_alpha.vp6");
	Video vid(stream);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	std::cout << "Done creating FileSystem: " << duration / 1000.0 << std::endl;
	t1 = t2;

	//Initialize ini system
	m_ini = std::make_unique<Ini>(*m_config,*m_fs);
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	std::cout << "Done INI parsing: " << duration / 1000.0 << std::endl;




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
