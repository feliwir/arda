#pragma once
#include <stdint.h>
#include <al.h>

namespace arda
{
	class AudioBuffer
	{
	public:
		AudioBuffer(int freq,ALenum fmt);
		~AudioBuffer();

		void Upload(uint8_t* data, int size);
	private:
		ALuint m_handle;
		ALenum m_format;
		int m_frequency;
	};
}