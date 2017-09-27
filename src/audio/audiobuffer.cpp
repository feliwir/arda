#include "audiobuffer.hpp"

arda::AudioBuffer::AudioBuffer(int freq,ALenum fmt) : 
	m_handle(0),
	m_frequency(freq),
	m_format(fmt)
{
	alGenBuffers(1, &m_handle);
}

arda::AudioBuffer::~AudioBuffer()
{
	if (m_handle)
		alDeleteBuffers(1, &m_handle);
}

void arda::AudioBuffer::Upload(uint8_t * data, int size)
{
	alBufferData(m_handle, m_format,data, size, m_frequency);
	ALenum err = alGetError();
}
