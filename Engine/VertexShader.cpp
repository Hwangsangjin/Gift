#include "pch.h"
#include "VertexShader.h"
#include "Renderer.h"

VertexShader::VertexShader(const wchar_t* file_path, Renderer* renderer)
	: m_renderer(renderer)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;

	::D3DCompileFromFile(file_path, nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &error_blob);
	assert(blob);

	m_renderer->GetD3DDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_vertex_shader);
	assert(m_vertex_shader);
}

ID3D11VertexShader* VertexShader::GetVertexShader() const
{
	return m_vertex_shader.Get();
}
