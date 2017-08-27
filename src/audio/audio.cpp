#include "audio.hpp"
#include "../core/config.hpp"
#include <iostream>
#include <al.h>
#include <cstring>
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

void arda::Audio::checkErrorAl(const std::string& message)
{
	ALenum error;
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw RuntimeException(message);
	}
}

void arda::Audio::checkErrorAlc(const std::string& message)
{
	ALenum error;
	error = alcGetError(m_internals->m_device);
	if (error != ALC_NO_ERROR)
	{
		throw RuntimeException(message);
	}
}

static void list_audio_devices(const ALCchar *devices)
{
        const ALCchar *device = devices, *next = devices + 1;
        size_t len = 0;

        fprintf(stdout, "Devices list:\n");
        fprintf(stdout, "----------\n");
        while (device && *device != '\0' && next && *next != '\0') {
                fprintf(stdout, "%s\n", device);
                len = strlen(device);
                device += (len + 1);
                next += (len + 2);
        }
        fprintf(stdout, "----------\n");
}

arda::Audio::Audio(Config& config) :
	m_internals(std::make_unique<AudioInternals>())
{
	auto& device = m_internals->m_device;
	auto& context = m_internals->m_context;
	
	ALboolean enumeration;
	
	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_TRUE)
		list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	device = alcOpenDevice(NULL);
	
	if(device==nullptr)
		checkErrorAlc("Failed to initialize OpenAL device");

	context = alcCreateContext(device,nullptr);
	
	if(context==nullptr)
		checkErrorAlc("Failed to create OpenAL context");
		
	if (!alcMakeContextCurrent(context))
		checkErrorAlc("Failed to make context current");
}

arda::Audio::~Audio()
{
	auto& device = m_internals->m_device;
	auto& context = m_internals->m_context;

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}
