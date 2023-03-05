#pragma once

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, Renderer* renderer);

	void SetFullScreen(bool fullscreen, UINT width, UINT height);
	void Resize(UINT width, UINT height);
	void Present(bool vsync);

	ID3D11RenderTargetView* GetRenderTargetView() const;
	ID3D11DepthStencilView* GetDepthStencilView() const;

private:
	void ReloadBuffers(UINT width, UINT height);

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_dxgi_swap_chain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_render_target_view = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depth_stencil_view = nullptr;

	Renderer* m_renderer = nullptr;
};

