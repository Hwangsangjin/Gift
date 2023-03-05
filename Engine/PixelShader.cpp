#include "pch.h"
#include "PixelShader.h"
#include "Graphics.h"

PixelShader::PixelShader(const wchar_t* file_path, Graphics* graphics)
    : m_graphics(graphics)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;

	::D3DCompileFromFile(file_path, nullptr, nullptr, "psmain", "ps_5_0", 0, 0, &blob, &error_blob);
	assert(blob);

	m_graphics->GetD3DDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pixel_shader);
	assert(m_pixel_shader);
}

ID3D11PixelShader* PixelShader::GetPixelShader() const
{
    return m_pixel_shader.Get();
}
