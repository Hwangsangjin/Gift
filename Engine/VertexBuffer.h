#pragma once

class VertexBuffer
{
public:
	VertexBuffer(void* vertex_list, UINT vertex_size, UINT vertex_count, Renderer* renderer);

	ID3D11Buffer* GetBuffer() const;
	ID3D11InputLayout* GetInputLayout() const;
	UINT GetVertexSize() const;
	UINT GetVertexCount() const;

private:
	UINT m_vertex_size = 0;
	UINT m_vertex_count = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_input_layout = nullptr;
	Renderer* m_renderer = nullptr;
};

