#pragma once
#include <string>
#include <memory>
#include <map>
#include "parser/context.hpp"

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

		void AddTemplate(std::shared_ptr<Template> temp, const std::string& name="");

		inline std::vector<std::string>& GetGlobalIncludes() { return m_globalIncludes; }
		std::shared_ptr<ParsingContext> GetContext(const std::string& path,bool load=true);
	private:
		FileSystem& m_fs;
		std::vector<std::string> m_globalIncludes;
		std::map<const std::string, std::shared_ptr<Template>> m_weapons;
		std::map<const std::string, std::shared_ptr<ParsingContext>> m_files;
	};
}