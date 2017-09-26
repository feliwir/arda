#include "d3d_renderer.hpp"
#include <GLFW/glfw3.h>
#include <d3d11.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "d3d_texture.hpp"
#include "d3d_layout.hpp"
#include "d3d_buffer.hpp"
#include "../../core/config.hpp"

arda::D3DRenderer::D3DRenderer(Config & c) : IRenderer(c)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	
	m_window = glfwCreateWindow(c.GetWidth(),
		c.GetHeight(),
		c.GetTitle().c_str(),
		0, 0);

	InitD3D(c);

	Resize(c.GetWidth(), c.GetHeight());

	RegisterCallbacks();
}

arda::D3DRenderer::~D3DRenderer()
{
	CleanD3D();
}

void arda::D3DRenderer::Clear()
{
	m_context->ClearRenderTargetView(m_backbuffer, m_clearColor);
}

void arda::D3DRenderer::Render()
{
}

void arda::D3DRenderer::Present()
{
	// switch the back buffer and the front buffer
	m_swapchain->Present(0, 0);
}

std::shared_ptr<arda::ITexture> arda::D3DRenderer::CreateTexture()
{
	return std::make_shared<D3DTexture>(0,0,m_device,m_context);
}

std::shared_ptr<arda::ITexture> arda::D3DRenderer::CreateTexture(Image & img)
{
	return std::make_shared<D3DTexture>(img,m_device, m_context);
}

void arda::D3DRenderer::InitD3D(Config& c)
{
	HWND handle = glfwGetWin32Window(m_window);

	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = handle;                              // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	UINT flags = c.IsDebug() ? D3D11_CREATE_DEVICE_DEBUG : NULL;

	D3D11CreateDeviceAndSwapChain(NULL,						// create a device, device context and swap chain using the information in the scd struct
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_swapchain,
		&m_device,
		NULL,
		&m_context);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_context->OMSetRenderTargets(1, &m_backbuffer, NULL);
}

void arda::D3DRenderer::CleanD3D()
{
	// close and release all existing COM objects
	m_swapchain->Release();
	m_device->Release();
	m_context->Release();
	m_backbuffer->Release();
}

std::shared_ptr<arda::Layout> arda::D3DRenderer::CreateLayout()
{
	return std::make_shared<D3DLayout>();
}

void arda::D3DRenderer::Draw(std::shared_ptr<IBuffer> vertices, std::shared_ptr<IBuffer> indices)
{
}

void arda::D3DRenderer::Resize(const int width, const int height)
{
	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	m_context->RSSetViewports(1, &viewport);
}

void arda::D3DRenderer::SetClearColor(const glm::vec4 & color)
{
	m_clearColor[0] = color.r;
	m_clearColor[1] = color.g;
	m_clearColor[2] = color.b;
	m_clearColor[3] = color.a;
}

std::shared_ptr<arda::IBuffer>arda::D3DRenderer::CreateBuffer(IBuffer::Type t, IBuffer::Usage u)
{
	return std::make_shared<D3DBuffer>(t, u);
}