#pragma once

class Graphics;

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT buffer_size, Graphics* graphics);
	~ConstantBuffer();

	void Update(DeviceContextPtr device_context, void* buffer);

	ID3D11Buffer* GetBuffer() const;

private:
	ID3D11Buffer* m_buffer = nullptr;
	Graphics* m_graphics = nullptr;
};

