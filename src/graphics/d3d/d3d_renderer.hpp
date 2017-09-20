#pragma once
#include "../renderer.hpp"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace arda
{
	class D3DRenderer final : public IRenderer
	{
	public:
		D3DRenderer(Config& c);
		~D3DRenderer();

		// Geerbt über IRenderer
		virtual void Clear() override;
		virtual void Render() override;
		virtual void Present() override;
		virtual std::shared_ptr<ITexture> CreateTexture() override;
		virtual std::shared_ptr<ITexture> CreateTexture(Image & img) override;
		// Geerbt über IRenderer
		virtual void SetClearColor(const glm::vec4 & color) override;

		// Geerbt über IRenderer
		virtual void Resize(const int width, const int height) override;
	private:
		void InitD3D(Config& c);
		void CleanD3D();
	private:
		IDXGISwapChain *m_swapchain;            // the pointer to the swap chain interface
		ID3D11Device *m_device;                 // the pointer to our Direct3D device interface
		ID3D11DeviceContext *m_context;			// the device context
		ID3D11RenderTargetView *m_backbuffer;
		float m_clearColor[4];


	};
}