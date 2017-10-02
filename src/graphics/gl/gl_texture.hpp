#pragma once
#include "../texture.hpp"
#include "flextGL.h"
#include <gli/gl.hpp>

namespace arda
{
	class GLTexture final : public ITexture
	{
	public:
		GLTexture(int width, int height);
		GLTexture(Image& img);
		~GLTexture(); 

		// Inherited via ITexture
		virtual void Update(Image& img) override;
		virtual void Bind(int i=-1) override;
		virtual void Generate() override;

	private:
		GLuint m_handle;

		static gli::gl s_gl;
	};
}