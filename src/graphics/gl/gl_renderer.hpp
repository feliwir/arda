#pragma once
#include "../renderer.hpp"
#include "flextGL.h"


namespace arda
{
	class GLRenderer final : public IRenderer
	{
	public:
		GLRenderer(Config& c);

		// Geerbt über IRenderer
		virtual void Clear() override;

		// Inherited via IRenderer
		virtual void Render() override;

		// Inherited via IRenderer
		virtual void Present() override;

		// Inherited via IRenderer
		virtual std::shared_ptr<ITexture> CreateTexture() override;
		virtual std::shared_ptr<ITexture> CreateTexture(Image & img) override;

		// Geerbt über IRenderer
		virtual void SetClearColor(const glm::vec4 & color) override;
		virtual void Resize(const int width, const int height) override;
	private:
		static void DebugCallback(GLenum source,
								GLenum type,
								GLuint id,
								GLenum severity,
								GLsizei length,
								const GLchar *message,
								const void *userParam);

		GLuint m_vao;
	};
}