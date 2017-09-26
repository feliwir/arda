#pragma once

namespace arda
{
	class IPlayable
	{
	public:
		enum State
		{
			STOPPED,
			PLAYING,
			PAUSED,
		};
	public:
		virtual void Start() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;

		virtual double GetPosition() = 0;

		inline double GetDuration()
		{
			return m_duration;
		}

		inline State GetState()
		{
			return m_state;
		}
	protected:
		State m_state;
		double m_duration;
	};
}