#pragma once

class IndexBuffer
{
public:
	IndexBuffer(void* index_list, UINT index_count, Graphics* graphics);

	UINT GetIndexCount() const;
	ID3D11Buffer* GetBuffer() const;

private:
	UINT m_index_count = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
	Graphics* m_graphics = nullptr;
};

