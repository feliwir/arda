#include "d3d_texture.hpp"
#include "../image.hpp"
#include "d3d_util.hpp"
#include <d3d11.h>
#include <gli/convert.hpp>

gli::dx arda::D3DTexture::s_dx;

arda::D3DTexture::D3DTexture(int width, int height, ID3D11Device * dev, ID3D11DeviceContext* context) :
	ITexture(width,height), m_handle(nullptr)
	,m_view(nullptr), m_device(dev)
	,m_context(context)
{
	Generate();
}

arda::D3DTexture::D3DTexture(Image & img, ID3D11Device * dev, ID3D11DeviceContext* context) :
	ITexture(img), m_handle(nullptr)
	, m_view(nullptr), m_device(dev)
	, m_context(context)
{
	Generate();
	Update(img);
}

arda::D3DTexture::~D3DTexture()
{
	if (m_device)
		m_device = nullptr;

	if (m_view)
	{
		m_view->Release();
		m_view = nullptr;
	}

	if (m_handle)
	{
		m_handle->Release();
		m_handle = nullptr;
	}
}

void arda::D3DTexture::Update(Image & img)
{
	gli::texture tex = img.GetTexture();

	if (tex.format() == gli::FORMAT_RGB8_UNORM_PACK8)
	{
		gli::texture2d input(tex);

		//D3D11 requires RGBA
		tex = gli::convert(input, gli::FORMAT_RGBA8_UNORM_PACK8);
	}

	gli::dx::format d3d_fmt = s_dx.translate(tex.format());

	D3D11_TEXTURE2D_DESC tex_desc;
	ZeroMemory(&tex_desc, sizeof(D3D11_TEXTURE2D_DESC));
	tex_desc.Width = m_width;
	tex_desc.Height = m_height;
	//we have a complete mipchain in that case
	tex_desc.MipLevels = (tex.levels()>1) ? 0 : 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = static_cast<DXGI_FORMAT>(d3d_fmt.DXGIFormat.DDS);
	tex_desc.SampleDesc.Count = 1;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_desc.MiscFlags = 0;

	int BPP = 4;
	if(!gli::is_compressed(tex.format()))
		BPP = gli::block_size(tex.format());
	else
		BPP = gli::block_extent(tex.format()).x;

	std::vector<D3D11_SUBRESOURCE_DATA> initial_data(tex.levels());

	//trigger this for mipmaps
	int level = 0;
	for (auto&& level_data : initial_data)
	{
		glm::tvec3<int> extent(tex.extent(level));
		int rowPitch = extent.x* BPP;
		ZeroMemory(&level_data, sizeof(D3D11_SUBRESOURCE_DATA));
		level_data.pSysMem = tex.data(0, 0, level);
		level_data.SysMemPitch = rowPitch;
		++level;
	}

	d3d::checkError(m_device->CreateTexture2D(&tex_desc, initial_data.data(), &m_handle),
					"Failed to create D3D Texture 2D");

	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	ZeroMemory(&view_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	// Setup the shader resource view description.
	view_desc.Format = tex_desc.Format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MostDetailedMip = tex.base_level();
	view_desc.Texture2D.MipLevels = tex.levels();
	
	// Create the shader resource view for the texture.
	d3d::checkError(m_device->CreateShaderResourceView(m_handle, &view_desc, &m_view),
					"Failed to create D3D Shader Resource View");
}

void arda::D3DTexture::Bind()
{

}

void arda::D3DTexture::Generate()
{
	//not doing anything actually
}
