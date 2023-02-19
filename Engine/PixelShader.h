#pragma once

class PixelShader
{
public:
	PixelShader(const void* shader_byte_code, size_t byte_code_size, Graphics* graphics);
	~PixelShader();

	ID3D11PixelShader* GetPixelShader() const;

private:
	ID3D11PixelShader* m_pixel_shader = nullptr;
	Graphics* m_graphics = nullptr;
};

