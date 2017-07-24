#pragma once
#include <string>

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
		AudioInternals* m_internals;
	};
}