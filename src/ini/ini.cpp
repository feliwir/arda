#include "ini.hpp"
#include "../filesystem/filesystem.hpp"
#include "../core/config.hpp"
#include "../filesystem/directory.hpp"
#include "../filesystem/file.hpp"
#include "../filesystem/stream.hpp"
#include "parser/lexer.hpp"
#include "parser/parser.hpp"
#include <iostream>

arda::Ini::Ini(Config & config, FileSystem & fs) : m_fs(fs)
{
	auto entry = fs.getEntry("data/ini");

	int num = 0;
	Parser p(*this);
	Lexer l(*this, m_fs);

	//walk down the directory recursively
	std::function<void(std::shared_ptr<IEntry>,const std::string& path)> recurse;
	recurse = [this,&p,&recurse,&num,&l](std::shared_ptr<IEntry> e,const std::string& path)
	{
		if (IEntry::isRegular(*e))
		{
			auto file = std::static_pointer_cast<File>(e);
			auto s = file->getStream();

			m_files.emplace(path, l.Lex(s, path));
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

	auto& macros = ParsingContext::GetGlobalMacros();

	for (const auto& path : m_globalIncludes)
	{
		auto context = l.Lex(fs.getStream(path), path);
		p.Parse(context);

		auto& m = context->GetMacros();
		macros.insert(m.begin(), m.end());

		m_files.emplace(path, context);
	}

	recurse(entry,"data/ini");

	//clear macro cache
	for (const auto& f : m_files)
	{
		auto& context = f.second;

		context->GetMacros().clear();
		p.Parse(context);
		context->GetTokenStream()->Clear();
	}
	macros.clear();

	std::cout << num << std::endl;
}

arda::Ini::~Ini()
{
}


std::shared_ptr<arda::ParsingContext> arda::Ini::GetContext(const std::string & path,bool load)
{
	auto it = m_files.find(path);
	if (it != m_files.end())
		return it->second;
	else if (load)
	{
		auto s = m_fs.getStream(path);
		Lexer l(*this, m_fs);
		m_files.emplace(path, l.Lex(s, path));
		return m_files[path];
	}
	else
		return nullptr;
	
}
