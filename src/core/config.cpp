#include "config.hpp"
#include <rapidjson/rapidjson.h>
#include <cxxopts.hpp>

arda::Config::Config(const std::vector<std::string>& args) :
	m_title(ENGINE_NAME),
	m_width(800),
	m_height(600),
	m_debug(false),
	m_root("."),
	m_gfxApi(0),
	m_iniDir("data/ini"),
	m_debugPort(2017),
	m_windowed(true)
{
	cxxopts::Options options("Arda Engine", "A modern reimplementation of the SAGE engine");
	options.add_options()
		("d,debug", "Enable debugging (default false)")
		("w,windowed", "Should start in windowed mode (default true)")
		("r,root", "Set the root directory of the original game", cxxopts::value<std::string>())
		("g,gfx-api", "Set the graphics api: 0=OpenGL (default),1=Direct3D 11",cxxopts::value<int>())
		("port", "Set the debug port (default 2017)", cxxopts::value<uint16_t>());

	//get back argc and argv from our argument vector
	int argc = args.size();
	std::vector<char*> cstrings;
	for(auto& arg : args)
		cstrings.push_back(const_cast<char*>(arg.c_str()));

	char** argv = cstrings.data();
	options.parse(argc, argv);

	if (options["root"].count() > 0)
		m_root = options["root"].as<std::string>();
	if (options["debug"].count() > 0)
		m_debug = options["debug"].as<bool>();
	if (options["gfx-api"].count() > 0)
		m_gfxApi = options["gfx-api"].as<int>();
	if (options["port"].count() > 0)
		m_debugPort = options["port"].as<uint16_t>();
	if(options["windowed"].count() > 0)
		m_windowed = options["windowed"].as<bool>();
}

arda::Config::~Config()
{
}

void arda::Config::Update()
{
}
