#pragma once

class PixelShader
{
public:
	PixelShader(const wchar_t* file_path, RenderSystem* render_system);

	ID3D11PixelShader* GetPixelShader() const;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixel_shader = nullptr;
	RenderSystem* m_render_system = nullptr;
};

