#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "../gui/gui.hpp"

namespace arda
{
	class Config;
	class Graphics;
	class Ini;
	class State;
	class FileSystem;

	typedef std::function<std::shared_ptr<State>()> StateConstruct;

	//Game logic is done here
    class Game
    {
	public:
		Game(Config& c, Graphics& g, Ini& i,FileSystem& fs);

		bool Update();
	private:
		//second parameter only required when speech!=movie
		std::shared_ptr<State> CreateCutscene(std::string_view name);
		std::shared_ptr<State> CreateTitlescreen();
	private:
		Config& m_config;
		Graphics& m_graphics;
		Ini& m_ini;
		FileSystem& m_fs;
		std::unique_ptr<GUI> m_gui;

		std::vector<StateConstruct> m_constructors;
		std::shared_ptr<State> m_state;
		int m_currentState;
    };
}