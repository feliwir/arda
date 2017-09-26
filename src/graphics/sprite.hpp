#pragma once
#include "drawable.hpp"
#include <memory>
#include <glm/glm.hpp>

namespace arda
{
	class IBuffer;
	class ITexture;
	class IRenderer;
	class Layout;

	class Sprite : public IDrawable
	{
	private:
		struct Vertex
		{
			glm::vec2 Position;
		};

	public:
		Sprite(IRenderer& renderer);
		Sprite(IRenderer& renderer,std::shared_ptr<ITexture> tex);
		~Sprite();

		// Inherited via IDrawable
		virtual void Render(IRenderer& renderer) override;

		inline void SetTexture(std::shared_ptr<ITexture> tex)
		{
			m_texture = tex;
		}
	private:
		std::shared_ptr<IBuffer> m_positions;
		std::shared_ptr<IBuffer> m_indices;
		std::shared_ptr<ITexture> m_texture;
		std::shared_ptr<Layout> m_layout;
		
	};
}