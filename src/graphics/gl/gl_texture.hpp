#pragma once
#include "../texture.hpp"
#include "../image.hpp"
#include "flextGL.h"

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
		virtual void Bind() override;
		virtual void Generate() override;

	private:
		GLuint m_handle;
	};
}