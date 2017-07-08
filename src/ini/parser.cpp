#include "parser.hpp"
#include "../core/exception.hpp"
#include "../filesystem/stream.hpp"
#include "../filesystem/filesystem.hpp"
#include "template.hpp"
#include <string>
#include <algorithm>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <chrono>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

inline std::string trim(const std::string& str,
	const std::string& whitespace = " ")
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

void arda::Parser::Parse(std::shared_ptr<IStream> s,const std::string& path,const FileSystem& fs, std::map<std::string, std::string>& macros)
{
	auto it = m_cache.find(path);

	if (it == m_cache.end())
		Parse(s->readAll(), path, fs, macros);
	else
		macros.insert(it->second.begin(), it->second.begin());
}

void arda::Parser::Parse(std::shared_ptr<IStream> stream, const std::string & path, const FileSystem & fs)
{
	std::map<std::string, std::string> macros;

	Parse(stream, path, fs, macros);
}

void arda::Parser::ClearCache()
{
	m_cache.clear();
}

void arda::Parser::Parse(const std::string & content, const std::string & path, const FileSystem & fs, std::map<std::string, std::string>& macros)
{

	auto it = m_cache.find(path);
	if (it != m_cache.end())
		macros.insert(it->second.begin(), it->second.end());

	std::stringstream sourcestream;
	sourcestream << content;

	std::string basepath = path.substr(0, path.find_last_of('/') + 1);

	//skipws lambda
	auto skipws = [](const std::string& str, size_t& pos)
	{
		while (str.size()>pos && str.at(pos) == ' ')
			++pos;
	};

	auto getmacro = [&](std::string& macro) 
	{
		if (!std::isdigit(macro.front()))
		{
			macro = macros[macro];
		}
	};

	std::function<std::string(const std::string&)> preprocess;
	preprocess = [&](const std::string& str) -> std::string
	{
		size_t beg = 0, end;
		std::string directive, first, second;
		end = str.find_first_of(" (");

		if (end == std::string::npos)
			return "";

		directive = str.substr(beg, end - beg);

		if (directive == "#define")
		{
			skipws(str, end);
			beg = end;
			end = str.find_first_of(' ', beg);
			first = str.substr(beg, end - beg);
			skipws(str, end);
			beg = end;
			end = str.size();
			second = str.substr(beg, end - beg);

			if (second.front() == '#')
				second = preprocess(second);

			macros[first] = second;
		}
		else if (directive == "#include")
		{
			beg = str.find_first_of('\"', end);
			end = str.find_first_of('\"', beg + 1);
			first = str.substr(beg + 1, end - (beg + 1));
			auto inc_path = basepath + first;
			inc_path = fs::weakly_canonical(fs::path(inc_path)).string();
			std::replace(inc_path.begin(), inc_path.end(), '\\', '/');
			std::transform(inc_path.begin(), inc_path.end(), inc_path.begin(), ::tolower);
			auto stream = fs.getStream(inc_path);
			
			if(stream)
				Parse(stream, inc_path, fs, macros);
		}
		else if (directive == "#ADD")
		{
			++end;
			skipws(str, end);
			beg = end;
			end = str.find_first_of(' ', beg);
			first = str.substr(beg, end - beg);
			skipws(str, end);
			beg = end;
			end = str.find_first_of(' ', beg);
			second = str.substr(beg, end - beg);
			
			getmacro(first);
			getmacro(second);
			if (first.size() == 0) { first = "0"; }
			if (second.size() == 0) { second = "0"; }

			std::string suffix = "";
			if (first.back() == '%' || second.back() == '%')
				suffix = "%";

			int sum = std::stoi(first) + std::stoi(second);
			return std::to_string(sum) + suffix;
		}
		else
		{
			int a = 0;
		}
		return "";
	};


	std::string line, first, second, third;
	std::string name,type;
	std::shared_ptr<Template> temp;
	int level = 0;
	size_t pos, beg, end;

	while (std::getline(sourcestream, line))
	{
		//remove all \r
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

		//replace tabs with spaces
		std::replace(line.begin(), line.end(), '\t', ' ');

		if (line.size() == 0)
			continue;

		//remove comments
		pos = line.find(';');
		if (pos != std::string::npos)
		{
			line = line.substr(0, pos);
		}

		if (line.size() == 0)
			continue;

		pos = line.find("//");
		if (pos != std::string::npos)
		{
			line = line.substr(0, pos);
		}

		if (line.size() == 0)
			continue;

		//remove trailing whitespaces
		boost::algorithm::trim(line);

		if (line.size() == 0)
			continue;

		//preprocessor
		if (line.front() == '#')
		{
			preprocess(line);
			continue;
		}

		pos = line.find_first_of(" =");

		//either an anonymous block or End
		if (pos == std::string::npos)
		{
			//end of block
			if (line == "End")
			{
				level--;
				temp = nullptr;
			}
			//unnamed block
			else
			{
				temp = Template::Create(line);
				level++;
			}
			continue;
		}

		//property/block name
		first = line.substr(0, pos);

		//skip all whitespaces
		skipws(line, pos);

		//property set
		if (line[pos] == '=')
		{
			++pos;
			skipws(line, pos);
			second = line.substr(pos, line.size());
			if(temp)
				temp->SetProperty(first, second);
		}
		//named block
		else
		{
			second = line.substr(pos, line.size());
			temp = Template::Create(first);
			name = second;
			level++;
		}
	}

	m_cache[path] = macros;
	std::cout << level << std::endl;
}
