#include "titlescreen.hpp"
#include "../../filesystem/filesystem.hpp"
#include "../../ini/ini.hpp"
#include "../../graphics/graphics.hpp"
#include "../../graphics/sprite.hpp"
#include "../../graphics/image.hpp"
#include "../../graphics/transitions/blend.hpp"
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

	m_video = std::make_shared<Video>(stream,true);
	auto tex = m_graphics.GetRenderer().CreateTexture();
	auto mask = m_graphics.GetRenderer().CreateTexture();

	float x = 0.075;
	float y = 0.76;
	m_ring = m_graphics.CreateSprite(tex, { {-x,-x-y},{x,-x-y},{-x,x-y},{x,x-y} },mask);

	m_ring->SetOpacity(0.0);
	ren.AddDrawable(m_ring);

	m_blending = std::make_shared<BlendAnimation>(
		std::vector<BlendAnimation::Keyframe>{
		{0,0.0},
		{1000,0.0},
		{3000,1.0},
		{4000,1.0}
		}, 
		std::vector<std::shared_ptr<Sprite>>{m_title_ea},
		std::vector<std::shared_ptr<Sprite>>{m_title_ring,m_ring});
}

arda::TitleScreen::~TitleScreen()
{
	auto& ren = m_graphics.GetRenderer();
	ren.RemoveDrawable(m_ring);
	ren.RemoveDrawable(m_title_ring);	
	ren.RemoveDrawable(m_title_ea);
}

void arda::TitleScreen::Start()
{
	m_blending->Start();
	m_video->Start();
}

void arda::TitleScreen::Update()
{
	m_blending->Update();
	
	m_ring->GetTexture()->Update(m_video->GetColorImage());
	m_ring->GetMask()->Update(m_video->GetAlphaImage());
}

bool arda::TitleScreen::IsFinished()
{
	return m_blending->IsFinished();
}
