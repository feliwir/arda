#pragma once
#include <memory>
#include "../media/playable.hpp"
#include "../graphics/image.hpp"

namespace arda
{
	class IStream;
	class VideoInternals;

	class Video final : public IPlayable
	{
	public:
		Video(std::shared_ptr<IStream> stream);
		~Video();

		// Geerbt über IPlayable
		virtual void Start() override;
		virtual void Pause() override;
		virtual void Stop() override;

		//all kinds of getters
		virtual double GetPosition() override;
		inline int GetWidth() { return m_width; }
		inline int GetHeight() { return m_height; }
		inline double GetFramerate() { return m_fps; }
		inline int GetCurrentFrame() { return m_curFrame; }
		inline int GetFrameCount() { return m_numFrames; }

		inline Image& GetColorImage() { return m_rgbImage; }
		inline Image& GetAlphaImage() { return m_alphaImage; }
	private:
		enum StreamIndex
		{
			COLOR = 0,
			ALPHA = 1
		};

		//will get a new color and a new alpha frame (when alpha is present)
		bool GetFrames();
	private:
		std::unique_ptr<VideoInternals> m_internals;
		bool m_hasAlpha;
		double m_fps;
		//in microseconds
		int m_frameTime;
		double m_position;
		int m_numFrames;
		int m_curFrame;
		int m_width;
		int m_height;
		Image m_rgbImage;
		Image m_alphaImage;
	};
}