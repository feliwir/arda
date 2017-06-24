#include "parser.hpp"
#include "../core/exception.hpp"
#include "../filesystem/stream.hpp"
#include "../filesystem/filesystem.hpp"
#include <string>
#include <algorithm>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <chrono>

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

void arda::Parser::Parse(std::shared_ptr<IStream> s,const std::string& path,const FileSystem& fs)
{
	std::string buffer(s->getSize(), ' ');
	s->read(&buffer[0], s->getSize());
	Parse(buffer,path,fs);
}

void arda::Parser::Parse(const std::string& ini, const std::string& path,const FileSystem& fs)
{
	std::stringstream sourcestream(ini);
	
	std::map<std::string, std::string> macros;
	std::string basepath = path.substr(0,path.find_last_of('/')+1);

	//skipws lambda
	auto skipws = [](const std::string& str,size_t& pos)
	{
		while (str.size()>pos && str.at(pos)==' ')
			++pos;
	};

	std::function<std::string(const std::string&)> preprocess;
	preprocess = [&,basepath](const std::string& str) -> std::string
	{
		size_t beg = 0, end;
		std::string directive,first, second;
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
			beg = str.find_first_of('\"',end);
			end = str.find_first_of('\"', beg+1);
			first = str.substr(beg+1, end - (beg+1));
			std::replace(first.begin(), first.end(), '\\', '/');
			int a = 0;
		}

		return "";
	};


	std::string line, first, second, third;

	int level = 0;
	size_t pos, beg, end;

	while (std::getline(sourcestream, line))
	{
		//remove all \r
		line.erase(std::remove(line.begin(), line.end(),'\r'),line.end());

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

		//either an anonymous block or Ende
		if (pos == std::string::npos)
		{
			//end of block
			if (line == "End")
			{
				level--;			
			}
			//unnamed block
			else
			{
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
		}
		//named block
		else
		{
			second = line.substr(pos, line.size());
			level++;
		}
	}

	std::cout << level << std::endl;

}