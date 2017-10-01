#pragma once
#include "../parser/block.hpp"
#include <glm/glm.hpp>

namespace arda::ini
{
	class MappedImage final : public Block
	{
	public:
		MappedImage();

		inline std::string& GetTexture() { return m_texture; }
		inline glm::ivec2 GetTextureSize() { return glm::ivec2(m_texWidth, m_texHeight); }
		inline glm::ivec4 GetCoords() { return m_coords; }

		// Inherited via Block
		virtual void SetProperty(std::string_view name, const Property value) override;
		virtual void Register(Ini & ini, std::string_view name) override;
	private:
		std::string m_texture;
		int m_texWidth;
		int m_texHeight;
		glm::ivec4 m_coords;
		std::string m_status;
	};
}