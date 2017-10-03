#pragma once
#include <string_view>

namespace arda
{
	class Config;
	class Graphics;
	class FileSystem;

	class GUI
	{
	public:
		GUI(Config& c,Graphics& g,FileSystem& fs);
		~GUI();

		void Update();

		void SetActive(std::string_view name);
	private:
		Graphics& m_graphics;
		FileSystem& m_filesystem;
	};
}