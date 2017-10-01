#include "titlescreen.hpp"
#include "../../filesystem/filesystem.hpp"
#include "../../graphics/graphics.hpp"
#include "../../graphics/sprite.hpp"
#include "../../graphics/image.hpp"

arda::TitleScreen::TitleScreen(FileSystem& fs,Graphics& graphics) :
m_graphics(graphics)
{
	auto& ren = m_graphics.GetRenderer();
	auto stream = fs.GetStream("art/compiledtextures/ti/titlescreenuserinterface.jpg");
	Image title_ui(stream);
	auto tex = ren.CreateTexture(title_ui);
	m_title_ring = m_graphics.CreateSprite(tex);

	stream = fs.GetStream("art/compiledtextures/lo/load_w_ea.jpg");
	Image title_ea(stream);

	tex = ren.CreateTexture(title_ea);
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
