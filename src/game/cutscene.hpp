#pragma once
#include "state.hpp"

namespace arda
{
	class Video;
	class AudioStream;
	class Graphics;
	class Sprite;
	class ITexture;

	class Cutscene final : public State
	{
	public:
		Cutscene(std::shared_ptr<Video> video, std::shared_ptr<AudioStream> audio,Graphics& graphics);
		~Cutscene();

		// Geerbt über State
		virtual void Start() override;
		virtual void Update() override;
		virtual bool IsFinished() override;

	private:
		std::shared_ptr<Video> m_video;
		std::shared_ptr<AudioStream> m_audio;
		Graphics& m_graphics;
		std::shared_ptr<Sprite> m_sprite;
		std::shared_ptr<ITexture> m_texture;
		bool m_hasAudio;
	};
}