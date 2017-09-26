#pragma once
#include <memory>

namespace arda
{
	class IRenderer;

	class IDrawable
	{
	public:
		virtual void Render(IRenderer& renderer) = 0;
	};
}