#pragma once

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, Graphics* graphics);
	~SwapChain();

	void SetFullScreen(bool fullscreen, UINT width, UINT height);
	void Resize(UINT width, UINT height);
	void Present(bool vsync);

	ID3D11RenderTargetView* GetRenderTargetView() const;
	ID3D11DepthStencilView* GetDepthStencilView() const;

private:
	void LoadBuffers(UINT width, UINT height);

	IDXGISwapChain* m_dxgi_swap_chain = nullptr;
	ID3D11RenderTargetView* m_render_target_view = nullptr;
	ID3D11DepthStencilView* m_depth_stencil_view = nullptr;
	Graphics* m_graphics = nullptr;
};

