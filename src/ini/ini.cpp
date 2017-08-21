#include "ini.hpp"
#include "../filesystem/filesystem.hpp"
#include "../core/config.hpp"
#include "../filesystem/directory.hpp"
#include "../filesystem/file.hpp"
#include "../filesystem/stream.hpp"
#include "parser/lexer.hpp"
#include "parser/parser.hpp"
#include "template.hpp"
#include <iostream>

arda::Ini::Ini(Config & config, FileSystem & fs)
{
	auto entry = fs.getEntry("data/ini");

	Parser p;
	int num = 0;

	//walk down the directory recursively
	std::function<void(std::shared_ptr<IEntry>,const std::string& path)> recurse;
	recurse = [this,&p,&recurse,&num,&fs](std::shared_ptr<IEntry> e,const std::string& path)
	{
		if (IEntry::isRegular(*e))
		{
			auto file = std::static_pointer_cast<File>(e);
			auto s = file->getStream();

			m_files.emplace(path, Lexer::Lex(s, path,*this,fs));
			++num;
		}
		else if (IEntry::isDirectory(*e))
		{
			auto dir = std::static_pointer_cast<Directory>(e);
			for (auto& e : dir->getEntries())
			{
				if (e.first == "." || e.first == "..")
					continue;
				
				recurse(e.second,path+"/"+e.first);
			}
		}
	};

	m_globalIncludes = { "data/ini/gamedata.ini" };

	for (const auto& p : m_globalIncludes)
	{
		m_files.emplace(p, Lexer::Lex(fs.getStream(p), p, *this,fs));
	}

	recurse(entry,"data/ini");
	std::cout << num << std::endl;
}

arda::Ini::~Ini()
{
}

void arda::Ini::AddTemplate(std::shared_ptr<Template> temp, const std::string& name)
{
	switch (temp->GetType())
	{
	case Template::WEAPON:
		m_weapons[name] = temp;
		break;
	}
}

std::shared_ptr<arda::ParsingContext> arda::Ini::GetContext(const std::string & path, FileSystem& fs)
{
	auto it = m_files.find(path);
	if (it != m_files.end())
		return it->second;
	else
	{
		auto s = fs.getStream(path);
		m_files.emplace(path, Lexer::Lex(s, path, *this,fs));
	}
	return std::shared_ptr<ParsingContext>();
}
