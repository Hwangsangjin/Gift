#pragma once

class VertexShader
{
public:
	VertexShader(const wchar_t* file_path, Renderer* renderer);

	ID3D11VertexShader* GetVertexShader() const;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertex_shader = nullptr;
	Renderer* m_renderer = nullptr;
};

