#pragma once
#include <vector>
#include <string>
#include <memory>

struct GLFWwindow;

namespace arda
{
	class Audio;
	class Config;
	class FileSystem;
	class Global;
	class Graphics;
	class Ini;

	class Application
	{
	public:
		inline Application() : Application(std::vector<std::string>()) {}
		Application(const std::vector<std::string>& args);
		~Application();

		void Run();

		static inline Global& GetGlobal() { return *s_global; }
	private:
		void ShowSplash();

	private:
		GLFWwindow* m_window;
		std::unique_ptr<Audio> m_audio;
		std::unique_ptr<Ini> m_ini;
		std::unique_ptr<Config> m_config;
		std::unique_ptr<FileSystem> m_fs;
		std::unique_ptr<Graphics> m_graphics;
		static std::unique_ptr<Global> s_global;
	};

	inline  Global& GetGlobal()
	{
		return Application::GetGlobal();
	}
}