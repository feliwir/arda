#pragma once
#include <memory>
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
	private:
		std::unique_ptr<IRenderer> m_renderer;
	};
}