#pragma once
#include "drawable.hpp"
#include <memory>

namespace arda
{
	class Texture;

	class Sprite : public IDrawable
	{
	public:
		Sprite();
		Sprite(std::shared_ptr<Texture> tex);
		~Sprite();

		// Inherited via IDrawable
		virtual void Render() = 0;
	private:
		std::shared_ptr<Texture> m_texture;
		
	};
}