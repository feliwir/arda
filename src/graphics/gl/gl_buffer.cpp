#include "gl_buffer.hpp"

arda::GLBuffer::GLBuffer(Type t,Usage u) : Buffer(t,u)
	,m_handle(0)
	,m_target(getTarget())
	,m_usageFlag(getUsage())
{
	Generate();
}

arda::GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &m_handle);
}

void arda::GLBuffer::Upload(uint8_t * data, int bufSize)
{
	Bind();
	glBufferData(m_target, bufSize, data, m_usageFlag);
}

void arda::GLBuffer::Bind()
{

	glBindBuffer(m_target, m_handle);
}

void arda::GLBuffer::Generate()
{
	glGenBuffers(1, &m_handle);
}

GLenum arda::GLBuffer::getTarget()
{
	switch (m_type)
	{
	case VERTEX_BUFFER:
		return GL_ARRAY_BUFFER;
	case INDEX_BUFFER:
		return GL_ELEMENT_ARRAY_BUFFER;
	default:
		return GL_ARRAY_BUFFER;
	}
}

GLenum arda::GLBuffer::getUsage()
{
	switch (m_usage)
	{
	case STATIC:
		return GL_STATIC_DRAW;
	case DYNAMIC:
		return GL_DYNAMIC_DRAW;
	case STREAM:
		return GL_STREAM_DRAW;
	default:
		return GL_STATIC_DRAW;
	}
}
