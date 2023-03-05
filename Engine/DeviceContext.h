#pragma once

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, Renderer* renderer);

	void ClearRenderTarget(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha);
	//void ClearRenderTarget(const TexturePtr& render_target, float red, float green, float blue, float alpha);
	void ClearDepthStencil(const SwapChainPtr& swap_chain);
	//void ClearDepthStencil(const TexturePtr& depth_stencil);
	//void SetRenderTarget(const TexturePtr& render_target, const TexturePtr& depth_stencil);
	void SetViewport(UINT width, UINT height);
	void SetVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void SetVertexShader(const VertexShaderPtr& vertex_shader);
	void SetPixelShader(const PixelShaderPtr& pixel_shader);
	//void SetTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture_list, UINT texture_size);
	void SetTexture(const Texture2DPtr* texture_list, UINT texture_size);
	void SetConstantBuffer(const ConstantBufferPtr& constant_buffer);
	void SetIndexBuffer(const IndexBufferPtr& index_buffer);
	void DrawTriangleList(UINT vertex_count, UINT start_vertex_location);
	void DrawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT base_vertex_location);
	void DrawTriangleStrip(UINT vertex_count, UINT start_vertex_location);

	ID3D11DeviceContext* GetDeviceContext();

private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_device_context = nullptr;
	Renderer* m_renderer = nullptr;
};

