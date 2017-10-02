#include "titlescreen.hpp"
#include "../../filesystem/filesystem.hpp"
#include "../../ini/ini.hpp"
#include "../../graphics/graphics.hpp"
#include "../../graphics/sprite.hpp"
#include "../../graphics/image.hpp"
#include "../../core/exception.hpp"
#include "../../video/video.hpp"
#include <ratio>

arda::TitleScreen::TitleScreen(FileSystem& fs, Graphics& graphics, Ini& ini) :
	m_graphics(graphics)
{
	auto& ren = m_graphics.GetRenderer();
	auto mapped = m_graphics.GetMappedImage("TitleScreenLogo", fs, ini);
	m_title_ea = m_graphics.CreateSprite(mapped);

	mapped = m_graphics.GetMappedImage("TitleScreen", fs, ini);
	m_title_ring = m_graphics.CreateSprite(mapped);
	m_title_ring->SetOpacity(0.0);

	ren.AddDrawable(m_title_ring);
	ren.AddDrawable(m_title_ea);

	auto video_ini = ini.GetBlock<ini::Video>("SmallRing");

	if (video_ini == nullptr)
		throw RuntimeException("Invalid video block: SmallRing");

	auto stream = fs.GetStream("data/movies/" + video_ini->GetFilename() + ".vp6");
	//if the video doesn't exist, skip this cutscene
	if (stream == nullptr)
		throw RuntimeException("Invalid cutscene name: " + video_ini->GetFilename());

	m_video = std::make_shared<Video>(stream);
	auto tex = m_graphics.GetRenderer().CreateTexture();
	auto mask = m_graphics.GetRenderer().CreateTexture();

	float x = 0.1;
	float y = 0.76;
	m_ring = m_graphics.CreateSprite(tex, { {-x,-x-y},{x,-x-y},{-x,x-y},{x,x-y} },mask);

	m_ring->SetOpacity(0.0);
	ren.AddDrawable(m_ring);
}

arda::TitleScreen::~TitleScreen()
{
	auto& ren = m_graphics.GetRenderer();
	ren.RemoveDrawable(m_title_ring);	
	ren.RemoveDrawable(m_title_ea);
}

void arda::TitleScreen::Start()
{
	m_start = std::chrono::high_resolution_clock::now();
	m_video->Start();
}

void arda::TitleScreen::Update()
{
	auto now  = std::chrono::steady_clock::now();
	auto passed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start);
	
	if (passed.count() > 300)
	{
		auto dur = passed.count() - 1000;
		auto alpha = dur / 2000.0;
		alpha = glm::clamp(alpha, 0.0, 1.0);
		m_title_ea->SetOpacity(1.0 - alpha);
		m_title_ring->SetOpacity(alpha);
		m_ring->SetOpacity(alpha);
	}

	m_ring->GetTexture()->Update(m_video->GetColorImage());
	m_ring->GetMask()->Update(m_video->GetAlphaImage());
}

bool arda::TitleScreen::IsFinished()
{
	return false;
}
