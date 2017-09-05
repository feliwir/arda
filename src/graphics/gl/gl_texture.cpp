#include "gl_texture.hpp"

arda::GLTexture::GLTexture(int width, int height) : 
	ITexture(width,height), m_handle(0)
{
	Generate();
}

arda::GLTexture::GLTexture(Image & img) : 
	ITexture(img), m_handle(0)
{
	Generate();
	Update(img);
}

arda::GLTexture::~GLTexture()
{
	if (m_handle)
	{
		glDeleteTextures(1, &m_handle);
	}
}

void arda::GLTexture::Update(Image& img)
{
	Bind();
	auto fmt = img.GetFormat();
	GLuint gl_fmt;

	switch (fmt)
	{
	case Image::RGB:
		gl_fmt = GL_RGB;
		break;
	case Image::RGBA:
		gl_fmt = GL_RGBA;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, gl_fmt, img.GetWidth(), img.GetHeight(), 0, gl_fmt, GL_UNSIGNED_BYTE, img.GetBuffer());
}

void arda::GLTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
}

void arda::GLTexture::Generate()
{
	glGenTextures(1, &m_handle);
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
