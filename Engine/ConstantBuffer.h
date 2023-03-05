#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT buffer_size, Renderer* renderer);

	void Update(DeviceContextPtr device_context, void* buffer);

	ID3D11Buffer* GetBuffer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
	Renderer* m_renderer = nullptr;
};

