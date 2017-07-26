#pragma once
#include <string>
#include <memory>

namespace arda
{
	class Config;
	class AudioInternals;

	class Audio
	{
	public:
		Audio(Config& config);
		~Audio();
	private:
		void checkError(const std::string& msg);
	private:
		std::unique_ptr<AudioInternals> m_internals;
	};
}