#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "renderer.hpp"

namespace arda
{
	class Config;
	class Sprite;
	class ITexture;
	class FileSystem;
	class Ini;

	class Graphics
	{
	public:
		Graphics(Config&);

		void Clear();

		void Render();

		void Present();

		inline IRenderer& GetRenderer()
		{
			return *m_renderer;
		}

		void SetFullscreen(const bool full);

		void CenterWindow();

		void HideCursor();
		void ShowCursor();

		std::shared_ptr<Sprite> CreateSprite(std::shared_ptr<ITexture> tex=nullptr);
		std::shared_ptr<ITexture> GetTexture(std::string_view name, FileSystem& fs);
		std::shared_ptr<Sprite> GetMappedImage(std::string_view name, FileSystem& fs, Ini& ini);

		void ClearTextures();
	private:
		std::map<std::string, std::shared_ptr<ITexture>> m_textures;
		std::unique_ptr<IRenderer> m_renderer;
		glm::vec4 m_clearColor;
		std::string m_artFolder;
		std::string m_compiledFolder;
		std::string m_regularFolder;
		std::vector<std::string> m_texExtensions;
		std::vector<std::string> m_texFolders;
	};
}