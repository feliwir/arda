#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "renderer.hpp"

namespace arda
{
	class Config;
	class Sprite;
	class ITexture;

	class Graphics
	{
	public:
		Graphics(Config&);

		void Clear();

		void Render();

		void Present();

		inline IRenderer& GetRenderer()
		{
			return *m_renderer;
		}

		void SetFullscreen(const bool full);

		void CenterWindow();

		std::shared_ptr<Sprite> CreateSprite(std::shared_ptr<ITexture> tex=nullptr);

	private:
		std::unique_ptr<IRenderer> m_renderer;
		glm::vec4 m_clearColor;
	};
}