#include "gl_layout.hpp"

arda::GLLayout::GLLayout(): m_vao(0)
{
	glGenVertexArrays(1, &m_vao);
}

arda::GLLayout::~GLLayout()
{
	if (m_vao)
		glDeleteVertexArrays(1, &m_vao);
}

void arda::GLLayout::Enable(int i)
{
	glEnableVertexAttribArray(i);
}

void arda::GLLayout::VertexPointer(int index, int size, Type type, int stride, void * offset)
{
	GLenum gl_type = GL_FLOAT;

	switch (type)
	{
	case FLOAT:
		gl_type = GL_FLOAT;
		break;
	case BYTE:
		gl_type = GL_BYTE;
		break;
	case UBYTE:
		gl_type = GL_UNSIGNED_BYTE;
		break;
	}

	glVertexAttribPointer(index, size, gl_type, GL_FALSE, stride, offset);
}

void arda::GLLayout::Bind()
{
	glBindVertexArray(m_vao);
}
