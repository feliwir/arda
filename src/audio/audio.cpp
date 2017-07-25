#include "audio.hpp"
#include "../core/config.hpp"
#include <iostream>
#include <al.h>
#include <alc.h>
#include "../core/exception.hpp"

namespace arda
{
	class AudioInternals
	{
	public:
		ALCdevice* m_device;
		ALCcontext* m_context;
	};
}

void arda::Audio::checkError(const std::string& message)
{
	ALenum error;
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw RuntimeException(message);
	}
}

arda::Audio::Audio(Config& config) :
	m_internals(new AudioInternals)
{
	auto& device = m_internals->m_device;
	auto& context = m_internals->m_context;

	device = alcOpenDevice(nullptr);
	checkError("Failed to initialize OpenAL device");

	context = alcCreateContext(m_internals->m_device,nullptr);
	checkError("Failed to create OpenAL context");
		
}

arda::Audio::~Audio()
{
	auto& device = m_internals->m_device;
	auto& context = m_internals->m_context;

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	delete m_internals;
}
