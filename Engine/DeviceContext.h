#pragma once

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, Graphics* graphics);
	~DeviceContext();

	ID3D11DeviceContext* GetDeviceContext();
	void SetViewportSize(UINT width, UINT height);
	void SetVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void SetVertexShader(const VertexShaderPtr& vertex_shader);
	void SetPixelShader(const PixelShaderPtr& pixel_shader);
	void SetTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture_list, UINT texture_size);
	void SetTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture_list, UINT texture_size);
	void SetConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& constant_buffer);
	void SetConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer);
	void SetIndexBuffer(const IndexBufferPtr& index_buffer);
	void DrawTriangleList(UINT vertex_count, UINT start_vertex_location);
	void DrawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT base_vertex_location);
	void DrawTriangleStrip(UINT vertex_count, UINT start_vertex_location);
	void ClearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha);

private:
	ID3D11DeviceContext* m_device_context = nullptr;
	Graphics* m_graphics = nullptr;
};

