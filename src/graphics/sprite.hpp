#pragma once
#include "drawable.hpp"
#include <memory>
#include <vector>
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
			glm::vec2 Coords;
		};

	public:
		Sprite(IRenderer& renderer);
		Sprite(IRenderer& renderer,std::shared_ptr<ITexture> tex, std::shared_ptr<ITexture> mask=nullptr);
		Sprite(IRenderer& renderer, std::shared_ptr<ITexture> tex,std::vector<glm::vec2> positions, std::shared_ptr<ITexture> mask=nullptr);
		Sprite(IRenderer& renderer, std::shared_ptr<ITexture> tex, std::vector<glm::vec2> positions, std::vector<glm::vec2> coords,std::shared_ptr<ITexture> mask = nullptr);
		~Sprite();

		// Inherited via IDrawable
		virtual void Render(IRenderer& renderer) override;

		inline void SetTexture(std::shared_ptr<ITexture> tex)
		{
			m_texture = tex;
		}

		inline void SetMask(std::shared_ptr<ITexture> mask)
		{
			m_mask = mask;
		}

		inline void SetOpacity(float opacity)
		{
			m_opacity = opacity;
		}
	private:
		std::shared_ptr<IBuffer> m_vertices;
		std::shared_ptr<IBuffer> m_indices;
		std::shared_ptr<ITexture> m_texture;
		std::shared_ptr<ITexture> m_mask;
		std::shared_ptr<Layout> m_layout;
		float m_opacity;
	};
}