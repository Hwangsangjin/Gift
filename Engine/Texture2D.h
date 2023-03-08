#pragma once

#include "Rect.h"

class Texture2D
{
public:
	enum class Type
	{
		Normal,
		RenderTarget,
		DepthStencil
	};

	Texture2D(const wchar_t* file_path, RenderSystem* render_system);
	Texture2D(const Rect& size, Texture2D::Type type, RenderSystem* render_system);

	ID3D11ShaderResourceView* GetShaderResourceView() const;
	ID3D11RenderTargetView* GetRenderTargetView() const;
	ID3D11DepthStencilView* GetDepthStencilView() const;
	ID3D11SamplerState* GetSamplerState() const;
	Rect GetSize() const;
	Texture2D::Type GetType() const;

//private:
	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shader_resource_view = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_render_target_view = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depth_stencil_view = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler_state = nullptr;

	Rect m_size = {};
	Texture2D::Type m_type = Texture2D::Type::RenderTarget;

	RenderSystem* m_render_system = nullptr;

	friend RenderSystem;
};

