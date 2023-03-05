#pragma once

class IndexBuffer
{
public:
	IndexBuffer(void* index_list, UINT index_count, Renderer* renderer);

	UINT GetIndexCount() const;
	ID3D11Buffer* GetBuffer() const;

private:
	UINT m_index_count = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
	Renderer* m_renderer = nullptr;
};

