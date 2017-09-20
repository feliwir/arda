#pragma once
#include <string_view>
#include <map>

namespace arda
{
    class Shader
    {
	public:
		virtual void Compile(std::string_view vert, std::string_view frag)=0;
		virtual void Compile(uint8_t* vertdata, int vertsize,
							 uint8_t* fragdata, int fragsize)=0;

		virtual void Bind() = 0;
	protected:
		std::map<std::string, int> m_uniforms;

		
    };
}