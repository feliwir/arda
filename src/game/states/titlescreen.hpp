#pragma once
#include "state.hpp"
#include <memory>

namespace arda
{
	class FileSystem;
	class Graphics;
	class Video;
	class Sprite;

	class TitleScreen final : public State
	{
	public:
		TitleScreen(FileSystem& fs,Graphics& graphics);
		~TitleScreen();

		// Geerbt �ber State
		virtual void Start() override;
		virtual void Update() override;
		virtual bool IsFinished() override;
	private:
		Graphics& m_graphics;
		std::shared_ptr<Video> m_video;
		std::shared_ptr<Sprite> m_title_ea;
		std::shared_ptr<Sprite> m_title_ring;
		std::shared_ptr<Sprite> m_ring;
	};
}