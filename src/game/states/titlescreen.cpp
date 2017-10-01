#include "titlescreen.hpp"
#include "../../filesystem/filesystem.hpp"
#include "../../graphics/graphics.hpp"
#include "../../graphics/sprite.hpp"
#include "../../graphics/image.hpp"

arda::TitleScreen::TitleScreen(FileSystem& fs,Graphics& graphics) :
m_graphics(graphics)
{
	auto& ren = m_graphics.GetRenderer();
	auto tex = m_graphics.GetTexture("titlescreenuserinterface",fs);
	m_title_ring = m_graphics.CreateSprite(tex);

	tex = m_graphics.GetTexture("load_w_ea",fs);
	m_title_ea = m_graphics.CreateSprite(tex);

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
