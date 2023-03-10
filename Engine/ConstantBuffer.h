#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT buffer_size, RenderSystem* renderer);

	void Update(const DeviceContextPtr& device_context, void* buffer);

	ID3D11Buffer* GetBuffer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
	RenderSystem* m_render_system = nullptr;
};

