#include "titlescreen.hpp"
#include "../../filesystem/filesystem.hpp"
#include "../../ini/ini.hpp"
#include "../../graphics/graphics.hpp"
#include "../../graphics/sprite.hpp"
#include "../../graphics/image.hpp"

arda::TitleScreen::TitleScreen(FileSystem& fs,Graphics& graphics,Ini& ini) :
m_graphics(graphics)
{
	auto& ren = m_graphics.GetRenderer();
	auto mapped = m_graphics.GetMappedImage("TitleScreen",fs,ini);
	m_title_ea = m_graphics.CreateSprite(mapped);

	mapped = m_graphics.GetMappedImage("TitleScreenLogo", fs, ini);
	m_title_ring = m_graphics.CreateSprite(mapped);

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
}

void arda::TitleScreen::Update()
{
}

bool arda::TitleScreen::IsFinished()
{
	return false;
}
