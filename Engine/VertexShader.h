#pragma once

class VertexShader
{
public:
	VertexShader(const void* shader_byte_code, size_t byte_code_size, Graphics* graphics);
	~VertexShader();

	ID3D11VertexShader* GetVertexShader() const;

private:
	ID3D11VertexShader* m_vertex_shader = nullptr;
	Graphics* m_graphics = nullptr;
};

