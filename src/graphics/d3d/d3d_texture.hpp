#pragma once
#include "../texture.hpp"
#include <gli/dx.hpp>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace arda
{
	class D3DTexture : public ITexture
	{
	public:
		D3DTexture(int width, int height, ID3D11Device* dev, ID3D11DeviceContext* context);
		D3DTexture(Image& img, ID3D11Device* dev, ID3D11DeviceContext* context);
		~D3DTexture();

		// Geerbt über ITexture
		virtual void Update(Image & img) override;
		virtual void Bind() override;
		virtual void Generate() override;
	private:
		ID3D11Texture2D* m_handle;
		ID3D11ShaderResourceView* m_view;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		static gli::dx s_dx;
	};
}