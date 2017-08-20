#pragma once
#include <string>
#include <memory>
#include <map>
#include "tokenstream.hpp"

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
	private:
		std::map<const std::string, std::shared_ptr<Template>> m_weapons;
		std::map<const std::string, TokenStream> m_lexed;
	};
}