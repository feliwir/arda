#pragma once
#include <string_view>
#include <memory>
#include <map>

namespace arda
{
	class Config;
	class Graphics;
	class FileSystem;
	class Apt;

	class GUI
	{
	public:
		GUI(Config& c,Graphics& g,FileSystem& fs);
		~GUI();

		void Update();

		void SetActive(std::string_view name);
	private:
		std::shared_ptr<Apt> LoadApt(std::string_view name);
	private:
		Graphics& m_graphics;
		FileSystem& m_fs;
		std::shared_ptr<Apt> m_active;

		std::map<std::string, std::shared_ptr<Apt>> m_apts;
	};
}