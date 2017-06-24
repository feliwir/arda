#pragma once
#include <vector>
#include <string>
#include <memory>

struct GLFWwindow;

namespace arda
{
	class Config;
	class FileSystem;
	class Global;
	class Ini;

	class Application
	{
	public:
		inline Application() : Application(std::vector<std::string>()) {}
		Application(const std::vector<std::string>& args);
		~Application();

		void Run();

		static inline Global& getGlobal() { return *s_global; }

	private:
		GLFWwindow* m_window;
		std::unique_ptr<Ini> m_ini;
		std::unique_ptr<Config> m_config;
		std::unique_ptr<FileSystem> m_fs;

		static std::unique_ptr<Global> s_global;
	};
}