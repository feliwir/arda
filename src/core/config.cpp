#include "config.hpp"
#include <rapidjson/rapidjson.h>
#include <cxxopts.hpp>

arda::Config::Config(const std::vector<std::string>& args) :
	m_title(ENGINE_NAME),
	m_width(800),
	m_height(600),
	m_debug(false),
	m_root("."),
	m_iniDir("data/ini"),
	m_debugPort(2017)
{
	cxxopts::Options options("Arda Engine", "A modern reimplementation of the SAGE engine");
	options.add_options()
		("d,debug", "Enable debugging")
		("r,root", "Set the root directory of the original game", cxxopts::value<std::string>())
		("port", "Set the debug port", cxxopts::value<uint16_t>());

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
	if (options["port"].count() > 0)
		m_debugPort = options["port"].as<uint16_t>();
}

arda::Config::~Config()
{
}

void arda::Config::Update()
{
}
