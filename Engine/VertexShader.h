#pragma once

class VertexShader
{
public:
	VertexShader(const wchar_t* file_path, Graphics* graphics);

	ID3D11VertexShader* GetVertexShader() const;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertex_shader = nullptr;
	Graphics* m_graphics = nullptr;
};

