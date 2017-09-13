#include "application.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include "config.hpp"
#include "global.hpp"
#include "debugger.hpp"
#include "../audio/audio.hpp"
#include "../video/video.hpp"
#include "../map/map.hpp"
#include "../graphics/graphics.hpp"
#include "../graphics/gl/gl_texture.hpp"
#include "../filesystem/filesystem.hpp"
#include "../filesystem/stream.hpp"
#include "../ini/ini.hpp"

std::unique_ptr<arda::Global> arda::Application::s_global;
std::shared_ptr<arda::GLTexture> s_tex;

arda::Application::Application(const std::vector<std::string>& args)
	: m_window(nullptr)
{

	//Load configuration first
	m_config = std::make_unique<Config>(args);

	//Initialize global variables
	s_global = std::make_unique<Global>(*m_config);

	//Initialize audio
	m_audio = std::make_unique<Audio>(*m_config);

	//Initialize graphics
	m_graphics = std::make_unique<Graphics>(*m_config);
	m_graphics->GetRenderer().SetClearColor({ 0.0, 0.0, 0.0, 1.0 });

	//Get the GLFW window
	m_window = m_graphics->GetRenderer().GetWindow();

	auto start = std::chrono::high_resolution_clock::now();

	//Initialize virtual filesystem
	m_fs = std::make_unique<FileSystem>(*m_config);

	auto stream = m_fs->GetStream("maps/map mp evendim/map mp evendim.map");
	Map map(stream);

	stream = m_fs->GetStream("GermanSplash.jpg");
	Image img(stream);
	s_tex = std::make_shared<GLTexture>(img);

	stream = m_fs->GetStream("data/movies/Credits_with_alpha.vp6");
	Video vid(stream);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	ARDA_LOG("Done creating FileSystem: " + std::to_string(duration / 1000.0));
	start = end;

	//Initialize ini system
	m_ini = std::make_unique<Ini>(*m_config,*m_fs);
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	ARDA_LOG("Done INI parsing: " + std::to_string(duration / 1000.0));

	glfwShowWindow(m_window);
}

arda::Application::~Application()
{

}

void arda::Application::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		m_graphics->Clear();

		glfwPollEvents();

		m_graphics->Present();
	}

}
