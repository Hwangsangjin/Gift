#include "pch.h"
#include "Texture.h"
#include "Engine.h"

Texture::Texture(const wchar_t* full_path)
	: Resource(full_path)
{	
	// 텍스처 생성
	DirectX::ScratchImage image_data;
	DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);
	DirectX::CreateTexture(Engine::GetInstance()->GetGraphics()->GetD3DDevice(), image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &m_texture);
	assert(m_texture);

	// 셰이더 리소스 뷰 구조체
	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = image_data.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = static_cast<UINT>(image_data.GetMetadata().mipLevels);
	desc.Texture2D.MostDetailedMip = 0;

	// 셰이더 리소스 뷰 생성
	Engine::GetInstance()->GetGraphics()->GetD3DDevice()->CreateShaderResourceView(m_texture, &desc, &m_shader_resource_view);
	assert(m_shader_resource_view);

	// 샘플러 구조체
	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = static_cast<FLOAT>(image_data.GetMetadata().mipLevels);

	// 샘플러 스테이트 생성
	Engine::GetInstance()->GetGraphics()->GetD3DDevice()->CreateSamplerState(&sampler_desc, &m_sampler_state);
	assert(m_sampler_state);
}

Texture::Texture(const Rect& rect, Texture::Type type)
	: Resource(L"")
{
	// 텍스처 타입에 따라서 바인딩
	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = rect.m_width;
	texture_desc.Height = rect.m_height;

	if (type == Texture::Type::Normal)
	{
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == Texture::Type::RenderTarget)
	{
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == Texture::Type::DepthStencil)
	{
		texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}

	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.MipLevels = 1;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.MiscFlags = 0;
	texture_desc.ArraySize = 1;
	texture_desc.CPUAccessFlags = 0;

	Engine::GetInstance()->GetGraphics()->GetD3DDevice()->CreateTexture2D(&texture_desc, nullptr, (ID3D11Texture2D**)&m_texture);
	assert(m_texture);

	if (type == Texture::Type::Normal || type == Texture::Type::RenderTarget)
	{
		Engine::GetInstance()->GetGraphics()->GetD3DDevice()->CreateShaderResourceView(m_texture, nullptr, &m_shader_resource_view);
		assert(m_shader_resource_view);
	}

	if (type == Texture::Type::RenderTarget)
	{
		Engine::GetInstance()->GetGraphics()->GetD3DDevice()->CreateRenderTargetView(m_texture, nullptr, &m_render_target_view);
		assert(m_render_target_view);
	}
	else if (type == Texture::Type::DepthStencil)
	{
		Engine::GetInstance()->GetGraphics()->GetD3DDevice()->CreateDepthStencilView(m_texture, nullptr, &m_depth_stencil_view);
		assert(m_depth_stencil_view);
	}
	
	m_rect = rect;
	m_type = type;
}

Texture::~Texture()
{
	if (m_sampler_state)
	{
		m_sampler_state->Release();
		m_sampler_state = nullptr;
	}

	if (m_depth_stencil_view)
	{
		m_depth_stencil_view->Release();
		m_depth_stencil_view = nullptr;
	}

	if (m_render_target_view)
	{
		m_render_target_view->Release();
		m_render_target_view = nullptr;
	}

	if (m_shader_resource_view)
	{
		m_shader_resource_view->Release();
		m_shader_resource_view = nullptr;
	}

	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() const
{
	return m_shader_resource_view;
}

ID3D11RenderTargetView* Texture::GetRenderTargetView() const
{
	return m_render_target_view;
}

ID3D11DepthStencilView* Texture::GetDepthStencilView() const
{
	return m_depth_stencil_view;
}

ID3D11SamplerState* Texture::GetSamplerState() const
{
	return m_sampler_state;
}

Rect Texture::GetRect() const
{
	return m_rect;
}

Texture::Type Texture::GetType() const
{
	return m_type;
}
