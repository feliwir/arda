#pragma once
#include "../shader.hpp"
#include "flextGL.h"
#include <array>

namespace arda
{
	class GLShader final : public Shader
	{
	public:
		GLShader();
		~GLShader();

		// Geerbt über Shader
		virtual void Compile(std::string_view vert, std::string_view frag) override;

		virtual void Compile(uint8_t * vertdata, int vertsize, uint8_t * fragdata, int fragsize) override;

		virtual void Bind() override;

		virtual void Link() override;

		virtual void AddUniform(std::string_view name) override;

		virtual void SetFloatProperty(std::string_view name, float value) override;
		virtual void SetIntegerProperty(std::string_view name, int value) override;
		virtual void SetBooleanProperty(std::string_view name, bool value) override;
	protected:
		virtual void Compile(ShaderType type, std::string_view src) override;
	private:
		GLuint m_program;
		std::array<GLuint, 5> m_shaders;
	};
}