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
		void checkErrorAl(const std::string& msg);
		void checkErrorAlc(const std::string& msg);
	private:
		std::unique_ptr<AudioInternals> m_internals;
	};
}