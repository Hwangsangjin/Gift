#include "pch.h"
#include "VertexShader.h"
#include "RenderSystem.h"

VertexShader::VertexShader(const wchar_t* file_path, RenderSystem* render_system)
	: m_render_system(render_system)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;

	::D3DCompileFromFile(file_path, nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &error_blob);
	assert(blob);

	m_render_system->GetD3DDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_vertex_shader);
	assert(m_vertex_shader);
}

ID3D11VertexShader* VertexShader::GetVertexShader() const
{
	return m_vertex_shader.Get();
}
