#include "pch.h"
#include "VertexShader.h"
#include "Graphics.h"

VertexShader::VertexShader(const void* shader_byte_code, size_t byte_code_size, Graphics* graphics)
	: m_graphics(graphics)
{
	if (FAILED(m_graphics->GetD3DDevice()->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vertex_shader)))
	{
		assert(m_vertex_shader);
		throw std::exception("VertexShader not created successfully");
	}
}

VertexShader::~VertexShader()
{
	if (m_vertex_shader)
	{
		m_vertex_shader->Release();
		m_vertex_shader = nullptr;
	}
}

ID3D11VertexShader* VertexShader::GetVertexShader() const
{
	return m_vertex_shader;
}
