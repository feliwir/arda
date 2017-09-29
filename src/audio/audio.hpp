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
	public:
		static void checkErrorAl(const std::string& msg);
	private:
		void checkErrorAlc(const std::string& msg);
	private:
		std::unique_ptr<AudioInternals> m_internals;
	};
}