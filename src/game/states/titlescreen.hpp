#pragma once
#include "state.hpp"
#include <memory>
#include <chrono>

namespace arda
{
	class FileSystem;
	class Graphics;
	class Video;
	class Sprite;
	class Ini;
	class BlendAnimation;

	class TitleScreen final : public State
	{
	public:
		TitleScreen(FileSystem& fs,Graphics& graphics,Ini& ini);
		~TitleScreen();

		// Geerbt �ber State
		virtual void Start() override;
		virtual void Update() override;
		virtual bool IsFinished() override;
	private:
		std::chrono::high_resolution_clock::time_point m_start;
		Graphics& m_graphics;
		std::shared_ptr<Video> m_video;
		std::shared_ptr<Sprite> m_title_ea;
		std::shared_ptr<Sprite> m_title_ring;
		std::shared_ptr<Sprite> m_ring;
		std::shared_ptr<BlendAnimation> m_blending;
	};
}