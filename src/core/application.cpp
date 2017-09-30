#include "application.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include "config.hpp"
#include "global.hpp"
#include "debugger.hpp"
#include "../audio/audio.hpp"
#include "../audio/audiostream.hpp"
#include "../video/video.hpp"
#include "../map/map.hpp"
#include "../game/game.hpp"
#include "../graphics/graphics.hpp"
#include "../graphics/sprite.hpp"
#include "../graphics/image.hpp"
#include "../graphics/gl/gl_texture.hpp"
#include "../filesystem/filesystem.hpp"
#include "../filesystem/stream.hpp"
#include "../filesystem/filestream.hpp"
#include "../ini/ini.hpp"
#include "../ini/blocks/ini_video.hpp"

std::mutex arda::Application::s_globalLock;
std::unique_ptr<arda::Global> arda::Application::s_global;
std::shared_ptr<arda::Sprite> s_splash;

arda::Application::Application(const std::vector<std::string>& args)
	: m_window(nullptr)
{
	s_globalLock.lock();

	//Load configuration first
	m_config = std::make_unique<Config>(args);

	//Initialize global variables
	s_global = std::make_unique<Global>(*m_config);
	s_globalLock.unlock();

	//Initialize audio
	m_audio = std::make_unique<Audio>(*m_config);

	//Initialize graphics
	m_graphics = std::make_unique<Graphics>(*m_config);
	m_graphics->CenterWindow();
	m_graphics->GetRenderer().SetClearColor({ 0.0, 0.0, 0.0, 1.0 });
	auto& ren = m_graphics->GetRenderer();
	
	ShowSplash();

	//Get the GLFW window
	m_window = m_graphics->GetRenderer().GetWindow();

	glfwShowWindow(m_window);

	auto start = std::chrono::high_resolution_clock::now();
	//Initialize virtual filesystem
	m_fs = std::make_unique<FileSystem>(*m_config);

	auto end = std::chrono::high_resolution_clock::now();

	// stream = m_fs->GetStream("maps/map mp evendim/map mp evendim.map");
	// Map map(stream);
	
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	ARDA_LOG("Done creating FileSystem: " + std::to_string(duration / 1000.0));
	start = end;

	//Initialize ini system
	m_ini = std::make_unique<Ini>(*m_config,*m_fs);
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	ARDA_LOG("Done INI parsing: " + std::to_string(duration / 1000.0));

	//check for an entry
	auto inivid = m_ini->GetBlock<ini::Video>("EALogoMovie");

	//m_graphics->SetFullscreen(true);

	m_game = std::make_unique<Game>(*m_config, *m_graphics, *m_ini,*m_fs);

	ren.RemoveDrawable(s_splash);
}

arda::Application::~Application()
{

}

void arda::Application::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		m_graphics->Clear();

		m_game->Update();

		m_graphics->Render();

		glfwPollEvents();

		m_graphics->Present();
	}

}

void arda::Application::ShowSplash()
{
	auto& ren = m_graphics->GetRenderer();

	std::shared_ptr<IStream> stream = std::make_shared<FileStream>(m_config->GetRootDir()+"/GermanSplash.jpg");
	Image img(stream);
	s_splash = m_graphics->CreateSprite(ren.CreateTexture(img));
	ren.AddDrawable(s_splash);
	m_graphics->Clear();
	m_graphics->Render();
	m_graphics->Present();

}
