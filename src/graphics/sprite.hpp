#pragma once
#include "drawable.hpp"
#include <memory>

namespace arda
{
	class Buffer;
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
		std::shared_ptr<Buffer> m_positions;
		std::shared_ptr<Buffer> m_indices;
		std::shared_ptr<Texture> m_texture;
		
	};
}