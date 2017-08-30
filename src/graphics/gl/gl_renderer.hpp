#pragma once
#include "../renderer.hpp"

namespace arda
{
	class GLRenderer : public IRenderer
	{
	public:
		GLRenderer();

		// Inherited via IRenderer
		virtual void Render() override;

		// Inherited via IRenderer
		virtual std::shared_ptr<ITexture> CreateTexture() override;
		virtual std::shared_ptr<ITexture> CreateTexture(Image & img) override;
	};
}