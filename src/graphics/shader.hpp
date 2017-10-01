#pragma once
#include <string_view>
#include <map>

namespace arda
{
    class Shader
    {
	protected:
		enum ShaderType
		{
			SH_VERTEX		= 0,
			SH_TESSC		= 1,
			SH_TESSE		= 2,
			SH_GEOMETRY		= 3,
			SH_FRAGMENT		= 4
		};

	protected:
		virtual void Compile(ShaderType type, std::string_view src) = 0;
	public:
		virtual void Compile(std::string_view vert, std::string_view frag)=0;
		virtual void Compile(uint8_t* vertdata, int vertsize,
							 uint8_t* fragdata, int fragsize)=0;

		virtual void Bind() = 0;

		virtual void Link() = 0;

		virtual void SetFloatProperty(std::string_view name, float value) = 0;

		virtual void AddUniform(std::string_view name) = 0;

		inline int GetUniform(std::string_view name)
		{
			return m_uniforms[std::string(name)];
		}
	protected:
		std::map<std::string, int> m_uniforms;		
    };
}