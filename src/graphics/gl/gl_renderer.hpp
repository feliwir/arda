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
	};
}