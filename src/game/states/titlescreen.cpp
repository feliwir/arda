#include "titlescreen.hpp"
#include "../../filesystem/filesystem.hpp"
#include "../../ini/ini.hpp"
#include "../../graphics/graphics.hpp"
#include "../../graphics/sprite.hpp"
#include "../../graphics/image.hpp"
#include <ratio>

arda::TitleScreen::TitleScreen(FileSystem& fs,Graphics& graphics,Ini& ini) :
m_graphics(graphics)
{
	auto& ren = m_graphics.GetRenderer();
	auto mapped = m_graphics.GetMappedImage("TitleScreenLogo",fs,ini);
	m_title_ea = m_graphics.CreateSprite(mapped);

	mapped = m_graphics.GetMappedImage("TitleScreen", fs, ini);
	m_title_ring = m_graphics.CreateSprite(mapped);
	m_title_ring->SetOpacity(0.0);

	ren.AddDrawable(m_title_ea);
	ren.AddDrawable(m_title_ring);
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
}

void arda::TitleScreen::Update()
{
	auto now  = std::chrono::steady_clock::now();
	auto passed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start);
	
	if (passed.count() > 1000)
	{
		auto dur = passed.count() - 1000;
		auto alpha = dur / 2000.0;
		alpha = glm::clamp(alpha, 0.0, 1.0);
		m_title_ea->SetOpacity(1.0 - alpha);
		m_title_ring->SetOpacity(alpha);
	}
}

bool arda::TitleScreen::IsFinished()
{
	return false;
}
