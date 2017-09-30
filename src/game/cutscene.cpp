#include "cutscene.hpp"
#include "../audio/audiostream.hpp"
#include "../video/video.hpp"
#include "../graphics/graphics.hpp"
#include "../graphics/sprite.hpp"

arda::Cutscene::Cutscene(std::shared_ptr<Video> video, std::shared_ptr<AudioStream> audio,Graphics& g)
	: m_video(video)
	, m_audio(audio)
	, m_hasAudio(m_audio!=nullptr)
	, m_graphics(g)
{
	//create a sprite and a texture for this cutscene
	auto& ren = m_graphics.GetRenderer();
	m_texture = ren.CreateTexture();
	m_sprite = m_graphics.CreateSprite(m_texture);
	ren.AddDrawable(m_sprite);
}

arda::Cutscene::~Cutscene()
{
	auto& ren = m_graphics.GetRenderer();
	ren.RemoveDrawable(m_sprite);
}

void arda::Cutscene::Start()
{
	m_graphics.HideCursor();
	m_video->Start();
	if(m_hasAudio)
		m_audio->Start();
}

void arda::Cutscene::Update()
{
	m_texture->Update(m_video->GetColorImage());
}

bool arda::Cutscene::IsFinished()
{
	bool finished = false;

	if (m_video->IsStopped())
	{
		if (m_hasAudio)
		{
			if (m_audio->IsStopped())
				finished = true;
		}
		else
			finished = true;
	}

	return finished;
}
