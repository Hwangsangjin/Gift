#pragma once

class PixelShader
{
public:
	PixelShader(const wchar_t* file_path, Graphics* graphics);

	ID3D11PixelShader* GetPixelShader() const;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixel_shader = nullptr;
	Graphics* m_graphics = nullptr;
};

