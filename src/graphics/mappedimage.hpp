#pragma once
#include <memory>
#include <vector>
#include <glm/vec2.hpp>

namespace arda
{
	class ITexture;

    class MappedImage
    {
	public:
		MappedImage(std::shared_ptr<ITexture> tex, std::vector<glm::vec2>& coords, 
			std::shared_ptr<ITexture> mask=nullptr);


		inline std::shared_ptr<ITexture> GetTexture()
		{
			return m_texture;
		}

		inline std::shared_ptr<ITexture> GetMask()
		{
			return m_mask;
		}

		inline std::vector<glm::vec2>& GetCoords()
		{
			return m_coords;
		}
	private:
		std::shared_ptr<ITexture> m_texture;
		std::shared_ptr<ITexture> m_mask;
		std::vector<glm::vec2> m_coords;
    };
}