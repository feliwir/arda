#include "gl_texture.hpp"
#include "../image.hpp"

gli::gl arda::GLTexture::s_gl(gli::gl::PROFILE_GL33);

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
	gli::texture tex = img.GetTexture();
	gli::gl::format const gl_fmt = s_gl.translate(tex.format(),tex.swizzles());
	GLenum target = s_gl.translate(tex.target());

	for (std::size_t level = 0; level < tex.levels(); ++level)
	{
		glm::tvec3<GLsizei> extent(tex.extent(level));
		if (gli::is_compressed(tex.format()))
		{
			glCompressedTexImage2D(target, level, gl_fmt.Internal, extent.x, extent.y, 0, tex.size(level), tex.data(0, 0, level));
		}
		else
		{
			glTexImage2D(target, level, GL_RGBA, extent.x, extent.y, 0, gl_fmt.External, gl_fmt.Type, tex.data(0, 0, level));
		}
	}		
}

void arda::GLTexture::Bind(int i)
{
	if (i >= 0)
		glActiveTexture(GL_TEXTURE0 + i);

	glBindTexture(GL_TEXTURE_2D, m_handle);
}

void arda::GLTexture::Generate()
{
	glGenTextures(1, &m_handle);
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
