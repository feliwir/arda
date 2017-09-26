#include "gl_shader.hpp"
#include "../../core/debugger.hpp"
#include <vector>

arda::GLShader::GLShader() : m_program(0)
	,m_shaders({0,0,0,0,0})
{
	m_program = glCreateProgram();
}

arda::GLShader::~GLShader()
{
	if (m_program)
		glDeleteProgram(m_program);

	for (const auto shader : m_shaders)
	{
		if(shader)
			glDeleteShader(shader);
	}
}

void arda::GLShader::Compile(std::string_view vert, std::string_view frag)
{
	Compile(SH_VERTEX, vert);
	Compile(SH_FRAGMENT, frag);
}

void arda::GLShader::Compile(uint8_t * vertdata, int vertsize, uint8_t * fragdata, int fragsize)
{
	
}

void arda::GLShader::Bind()
{
	glUseProgram(m_program);
}

void arda::GLShader::Link()
{
	//attach all shaders
	for (const auto shader : m_shaders)
	{
		if (shader)
			glAttachShader(m_program,shader);
	}

	glLinkProgram(m_program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_program, maxLength, &maxLength, infoLog.data());

		if (infoLog.size()>0)
			ARDA_LOG(infoLog.data());

		return;
	}

	//detach all shaders
	for (auto& shader : m_shaders)
	{
		if (shader)
			glDetachShader(m_program, shader);

		shader = 0;
	}
}

void arda::GLShader::Compile(ShaderType type, std::string_view src)
{
	GLenum gl_type = GL_VERTEX_SHADER;

	switch (type)
	{
	case SH_VERTEX:
		gl_type = GL_VERTEX_SHADER;
		break;
	case SH_TESSC:
		gl_type = GL_TESS_CONTROL_SHADER;
		break;
	case SH_TESSE:
		gl_type = GL_TESS_EVALUATION_SHADER;
		break;
	case SH_GEOMETRY:
		gl_type = GL_GEOMETRY_SHADER;
		break;
	case SH_FRAGMENT:
		gl_type = GL_FRAGMENT_SHADER;
		break;
	}

	GLuint shader = glCreateShader(gl_type);

	const GLchar* gl_src = src.data();
	glShaderSource(shader, 1, &gl_src, NULL);

	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength,errorLog.data());

		if(errorLog.size()>0)
			ARDA_LOG(errorLog.data());

		glDeleteShader(shader); // Don't leak the shader.
	}
	else
	{
		auto& id = m_shaders[type];

		if (id)
		{
			glDeleteShader(id);
			id = 0;
		}

		m_shaders[type] = shader;
	}
}

void arda::GLShader::AddUniform(std::string_view name)
{
	m_uniforms[std::string(name)] = glGetUniformLocation(m_program, name.data());
}
