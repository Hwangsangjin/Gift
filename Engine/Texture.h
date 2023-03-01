#pragma once

#include "Resource.h"
#include "Rect.h"

class Texture : public Resource
{
public:
	enum class Type
	{
		Normal,
		RenderTarget,
		DepthStencil
	};

	Texture(const wchar_t* full_path);
	Texture(const Rect& rect, Texture::Type type);
	virtual ~Texture() override;

	ID3D11ShaderResourceView* GetShaderResourceView() const;
	ID3D11RenderTargetView* GetRenderTargetView() const;
	ID3D11DepthStencilView* GetDepthStencilView() const;
	ID3D11SamplerState* GetSamplerState() const;
	Rect GetRect() const;
	Texture::Type GetType() const;

private:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_resource_view = nullptr;
	ID3D11RenderTargetView* m_render_target_view = nullptr;
	ID3D11DepthStencilView* m_depth_stencil_view = nullptr;
	ID3D11SamplerState* m_sampler_state = nullptr;

	Rect m_rect = {};
	Texture::Type m_type = Texture::Type::Normal;
};

