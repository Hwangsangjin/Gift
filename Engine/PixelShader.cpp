#include "pch.h"
#include "PixelShader.h"
#include "Graphics.h"

PixelShader::PixelShader(const void* shader_byte_code, size_t byte_code_size, Graphics* graphics)
    : m_graphics(graphics)
{
    if (FAILED(m_graphics->GetD3DDevice()->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_pixel_shader)))
    {
        assert(m_pixel_shader);
        throw std::exception("PixelShader not created successfully");
    }
}

PixelShader::~PixelShader()
{
    if (m_pixel_shader)
    {
        m_pixel_shader->Release();
        m_pixel_shader = nullptr;
    }
}

ID3D11PixelShader* PixelShader::GetPixelShader() const
{
    return m_pixel_shader;
}
