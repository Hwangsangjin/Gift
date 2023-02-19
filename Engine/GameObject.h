#pragma once

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	VertexShaderPtr m_vertex_shader = nullptr;
	PixelShaderPtr m_pixel_shader = nullptr;
	VertexBufferPtr m_vertex_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;
	ConstantBufferPtr m_constant_buffer = nullptr;
};

