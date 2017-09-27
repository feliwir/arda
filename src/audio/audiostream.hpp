#pragma once
#include <memory>
#include "../media/playable.hpp"

namespace arda
{
	class IStream;
	class AudioStreamInternals;;

	class AudioStream final : public IPlayable
	{
	public:
		AudioStream(std::shared_ptr<IStream> stream);
		~AudioStream();

		// Geerbt über IPlayable
		virtual void Start() override;
		virtual void Pause() override;
		virtual void Stop() override;
		virtual double GetPosition() override;
	private:
		void UpdateBuffers();
	private:
		std::unique_ptr<AudioStreamInternals> m_internals;
		int m_bitrate;
		int m_frequency;
		int m_channels;
		double m_position;
	};
}