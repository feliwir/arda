#pragma once
#include "../shader.hpp"
#include "flextGL.h"

namespace arda
{
	class GLShader final : public Shader
	{


	private:
		GLuint m_program;

	};
}