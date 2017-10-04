#include "gui.hpp"
#include "../filesystem/filesystem.hpp"
#include "../core/debugger.hpp"
#include "apt/apt.hpp"
#include "apt/constant.hpp"

arda::GUI::GUI(Config& c, Graphics& g, FileSystem& fs) :
	m_graphics(g),m_fs(fs)
{

}

arda::GUI::~GUI()
{
}

void arda::GUI::Update()
{
}

void arda::GUI::SetActive(std::string_view name)
{
	auto it = m_apts.find(std::string(name));
	std::shared_ptr<Apt> apt;

	if (it == m_apts.end())
	{
		apt = LoadApt(name);
	}

	m_apts.emplace(name, apt);
	m_active = apt;	
}

std::shared_ptr<arda::Apt> arda::GUI::LoadApt(std::string_view name)
{
	//load the corresponding streams:
	auto str_name = std::string(name);

	auto aptstream = m_fs.GetStream(str_name + ".apt");
	if (aptstream == nullptr)
	{
		ARDA_LOG("Missing .apt file: " + str_name + ".apt");
		return nullptr;
	}

	auto conststream = m_fs.GetStream(str_name + ".const");
	if (conststream == nullptr)
	{
		ARDA_LOG("Missing .const file: " + str_name + ".const");
		return nullptr;
	}

	auto datstream = m_fs.GetStream(str_name + ".dat");
	if (conststream == nullptr)
	{
		ARDA_LOG("Missing .dat file: " + str_name + ".dat");
		return nullptr;
	}

	//first load the constant part
	auto constData = std::make_shared<Constant>(conststream);

	//then load the apt
	auto apt = std::make_shared<Apt>(aptstream, constData);

	return std::shared_ptr<Apt>();
}
