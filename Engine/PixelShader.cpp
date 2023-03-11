#include "pch.h"
#include "PixelShader.h"
#include "RenderSystem.h"
#include "ShaderInclude.h"

PixelShader::PixelShader(const wchar_t* file_path, RenderSystem* render_system)
	: m_render_system(render_system)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;
	ShaderInclude shader_include(file_path);

	::D3DCompileFromFile(file_path, nullptr, &shader_include, "psmain", "ps_5_0", 0, 0, &blob, &error_blob);
	assert(blob);

	m_render_system->GetD3DDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pixel_shader);
	assert(m_pixel_shader);
}

ID3D11PixelShader* PixelShader::GetPixelShader() const
{
	return m_pixel_shader.Get();
}
