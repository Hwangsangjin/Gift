#include "pch.h"
#include "SwapChain.h"
#include "RenderSystem.h"

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* render_system)
    : m_render_system(render_system)
{
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.BufferCount = 1;
    desc.BufferDesc.Width = width > 0 ? width : 1;
    desc.BufferDesc.Height = height > 0 ? height : 1;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = hwnd;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    desc.Windowed = TRUE;

    auto device = m_render_system->GetD3DDevice();
    m_render_system->GetDXGIFactory()->CreateSwapChain(device, &desc, &m_dxgi_swap_chain);
    assert(m_dxgi_swap_chain);

    ReloadBuffers(width, height);
}

void SwapChain::SetFullScreen(bool fullscreen, UINT width, UINT height)
{
    Resize(width, height);
    m_dxgi_swap_chain->SetFullscreenState(fullscreen, nullptr);
}

void SwapChain::Resize(UINT width, UINT height)
{
    m_render_target_view.Reset();
    m_depth_stencil_view.Reset();

    m_dxgi_swap_chain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    ReloadBuffers(width, height);
}

void SwapChain::Present(bool vsync)
{
    m_dxgi_swap_chain->Present(vsync, NULL);
}

ID3D11RenderTargetView* SwapChain::GetRenderTargetView() const
{
    return m_render_target_view.Get();
}

ID3D11DepthStencilView* SwapChain::GetDepthStencilView() const
{
    return m_depth_stencil_view.Get();
}

void SwapChain::ReloadBuffers(UINT width, UINT height)
{
    ID3D11Texture2D* buffer = nullptr;
    m_dxgi_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
    assert(buffer);

    m_render_system->GetD3DDevice()->CreateRenderTargetView(buffer, nullptr, &m_render_target_view);
    buffer->Release();
    assert(m_render_target_view);

    D3D11_TEXTURE2D_DESC texture_desc = {};
    texture_desc.Width = width > 0 ? width : 1;
    texture_desc.Height = height > 0 ? height : 1;
    texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    texture_desc.Usage = D3D11_USAGE_DEFAULT;
    texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    texture_desc.MipLevels = 1;
    texture_desc.SampleDesc.Count = 1;
    texture_desc.SampleDesc.Quality = 0;
    texture_desc.MiscFlags = 0;
    texture_desc.ArraySize = 1;
    texture_desc.CPUAccessFlags = 0;

    m_render_system->GetD3DDevice()->CreateTexture2D(&texture_desc, nullptr, &buffer);
    assert(buffer);

    m_render_system->GetD3DDevice()->CreateDepthStencilView(buffer, nullptr, &m_depth_stencil_view);
    buffer->Release();
    assert(m_depth_stencil_view);
}
