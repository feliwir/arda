#pragma once
#include <string>
#include <memory>
#include <map>

namespace arda
{
	class Config;
	class FileSystem;
	class IEntry;
	class Template;

	class Ini
	{
	public:
		Ini(Config& config,FileSystem& fs);
		~Ini();

		void AddWeapon(const std::string name, std::shared_ptr<Template> weapon);
	private:
		std::map<const std::string, std::shared_ptr<Template>> m_weapons;
	};
}