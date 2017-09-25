#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include "drawable.hpp"
#include "texture.hpp"
#include "buffer.hpp"
#include "layout.hpp"
#include "shader.hpp"

struct GLFWwindow;

namespace arda
{
	class Config;

	class IRenderer
	{
	public:
		IRenderer(Config& c);
		~IRenderer();

		virtual void Clear() = 0;

		virtual void Render() = 0;

		virtual void Present() = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		
		inline void AddDrawable(std::shared_ptr<IDrawable> drawable)
		{
			m_drawables.push_back(drawable);
		}

		inline void RemoveDrawable(std::shared_ptr<IDrawable> drawable)
		{
			m_drawables.erase(std::remove(m_drawables.begin(), m_drawables.end(), drawable), m_drawables.end());
		}

		inline GLFWwindow* GetWindow()
		{
			return m_window;
		}

		virtual void Resize(const int width, const int height) = 0;

		virtual void Draw(std::shared_ptr<IBuffer> vertices, std::shared_ptr<IBuffer> indices) = 0;

		//Create all kinds of objects
		virtual std::shared_ptr<ITexture> CreateTexture() = 0;
		virtual std::shared_ptr<ITexture> CreateTexture(Image& img) = 0;
		virtual std::shared_ptr<IBuffer> CreateBuffer(IBuffer::Type, IBuffer::Usage) = 0;
		virtual std::shared_ptr<Layout> CreateLayout() = 0;
	protected:
		void RegisterCallbacks();		
	private:
		//callbacks
		static void CallbackResize(GLFWwindow* window, int width, int height);
	protected:
		GLFWwindow* m_window;

		std::unique_ptr<Shader> m_spriteShader;
		std::unique_ptr<Shader> m_modelShader;
	
		std::vector<std::shared_ptr<IDrawable>> m_drawables;
	};
}