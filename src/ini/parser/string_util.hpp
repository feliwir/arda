#pragma once
#include <string>

#include <string_view>
namespace arda
{
    template<char sep>
    static inline std::string_view ReadTill(std::string_view str,int& pos)
    {
        char c = str[pos];
		int first = pos;

        while (c!=sep && pos < str.size())
            c = str[++pos];
        
		int last = pos;

        return str.substr(first,last-first);
    }

    template<char sep>
    static inline void Skip(std::string_view str, int& pos)
    {
		pos = str.find_first_not_of(sep, pos);
    }
	
    static inline std::string_view Trim(std::string_view str)
    {
        size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first)
        {
			return "";
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
}