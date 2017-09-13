#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "renderer.hpp"
#include "image.hpp"

namespace arda
{
	class Config;

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
	private:
		std::unique_ptr<IRenderer> m_renderer;
		glm::vec4 m_clearColor;
	};
}