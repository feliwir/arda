#pragma once
#include <memory>

namespace arda
{
	class IStream;
	class VideoInternals;

	class Video
	{
	public:
		Video(std::shared_ptr<IStream> stream);
		~Video();
	private:
		std::unique_ptr<VideoInternals> m_internals;
		bool m_hasAlpha;
		float m_fps;
		int m_duration;
		int m_width;
		int m_height;
	};
}