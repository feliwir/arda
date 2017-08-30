#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "drawable.hpp"
#include "texture.hpp"

namespace arda
{
	class IRenderer
	{
	public:
		virtual void Render() = 0;
		inline void AddDrawable(std::shared_ptr<IDrawable> drawable)
		{
			m_drawables.push_back(drawable);
		}

		inline void RemoveDrawable(std::shared_ptr<IDrawable> drawable)
		{
			m_drawables.erase(std::remove(m_drawables.begin(), m_drawables.end(), drawable), m_drawables.end());
		}

		virtual std::shared_ptr<ITexture> CreateTexture() = 0;
		virtual std::shared_ptr<ITexture> CreateTexture(Image& img) = 0;
	private:
		std::vector<std::shared_ptr<IDrawable>> m_drawables;
	};
}