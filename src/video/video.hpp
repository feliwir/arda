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
	};
}