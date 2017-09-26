#pragma once
#include "../renderer.hpp"
#include "flextGL.h"


namespace arda
{
	class GLRenderer final : public IRenderer
	{
	public:
		GLRenderer(Config& c);

		// Geerbt �ber IRenderer
		virtual void Clear() override;

		// Inherited via IRenderer
		virtual void Render() override;

		// Inherited via IRenderer
		virtual void Present() override;

		// Geerbt �ber IRenderer
		virtual void SetClearColor(const glm::vec4 & color) override;
		virtual void Resize(const int width, const int height) override;

		virtual void Draw(std::shared_ptr<IBuffer> vertices, std::shared_ptr<IBuffer> indices) override;

		// Inherited via IRenderer
		virtual std::shared_ptr<ITexture> CreateTexture() override;
		virtual std::shared_ptr<ITexture> CreateTexture(Image & img) override;

		// Geerbt �ber IRenderer
		virtual std::shared_ptr<IBuffer> CreateBuffer(IBuffer::Type= IBuffer::VERTEX_BUFFER,
													  IBuffer::Usage= IBuffer::STATIC) override;

		// Geerbt �ber IRenderer
		virtual std::shared_ptr<Layout> CreateLayout() override;
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