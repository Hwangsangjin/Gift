#pragma once

class IndexBuffer
{
public:
	IndexBuffer(void* indices, UINT index_count, Graphics* graphics);
	~IndexBuffer();

	ID3D11Buffer* GetBuffer() const;
	UINT GetIndexCount() const;

private:
	UINT m_index_count = 0;

	ID3D11Buffer* m_buffer = nullptr;
	Graphics* m_graphics = nullptr;
};

