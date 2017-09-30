#include "game.hpp"
#include "../core/config.hpp"
#include "../core/exception.hpp"
#include "../ini/ini.hpp"
#include "../graphics/graphics.hpp"
#include "../graphics/sprite.hpp"
#include "../filesystem/filesystem.hpp"
#include "../audio/audiostream.hpp"
#include "../video/video.hpp"
#include "states/state.hpp"
#include "states/cutscene.hpp"
#include "states/titlescreen.hpp"

arda::Game::Game(Config & c, Graphics & g, Ini & i,FileSystem& fs) :
	m_config(c), m_graphics(g),m_ini(i), m_currentState(0),m_fs(fs)
{
	//set our game options
	if (!m_config.IsWindowed())
		m_graphics.SetFullscreen(true);


	m_constructors.push_back(std::bind(&Game::CreateCutscene, this, "EALogoMovie"));
	m_constructors.push_back(std::bind(&Game::CreateCutscene, this, "NewLineMovie"));
	m_constructors.push_back(std::bind(&Game::CreateCutscene, this, "TolkienLogo"));
	//m_constructors.push_back(std::bind(&Game::CreateCutscene, this, "Overall_Game_Intro"));
	m_constructors.push_back(std::bind(&Game::CreateTitlescreen, this));
}

bool arda::Game::Update()
{
	if (m_currentState == m_constructors.size())
		return false;

	if (m_state == nullptr)
	{
		auto& construct = m_constructors[m_currentState];
		m_state = construct();
		m_state->Start();
	}
	else
	{
		if (m_state->IsFinished())
		{
			m_state.reset();
			++m_currentState;

			if (m_currentState == m_constructors.size())
				return false;

			auto& construct = m_constructors[m_currentState];
			m_state = construct();
			m_state->Start();
			return true;
		}
	}

	m_state->Update();

	return true;
}

std::shared_ptr<arda::State> arda::Game::CreateCutscene(std::string_view name)
{
	//get the video block from the ini
	auto video_ini = m_ini.GetBlock<ini::Video>(name);

	auto audio_ini = m_ini.GetBlock<ini::Speech>(video_ini->GetFilename());

	if (video_ini == nullptr)
		throw RuntimeException("Invalid cutscene name: "+std::string(name));

	auto stream = m_fs.GetStream("data/movies/" + video_ini->GetFilename()+".vp6");
	//if the video doesn't exist, skip this cutscene
	if (stream == nullptr)
		return nullptr;

	auto video = std::make_shared<Video>(stream);

	std::shared_ptr<AudioStream> audio_stream;

	if (audio_ini)
	{
		stream = m_fs.GetStream("data/audio/speech/" + audio_ini->GetFilename());

		if (stream != nullptr)
		{
			audio_stream = std::make_shared<AudioStream>(stream);
			audio_stream->SetVolume(video_ini->GetVolume());
		}
	}
	
	auto cs = std::make_shared<Cutscene>(video, audio_stream,m_graphics);

	return cs;
}

std::shared_ptr<arda::State> arda::Game::CreateTitlescreen()
{
	auto title = std::make_shared<TitleScreen>(m_fs,m_graphics);

	return title;
}
