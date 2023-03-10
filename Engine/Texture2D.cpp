#include "pch.h"
#include "Texture2D.h"
#include "RenderSystem.h"

Texture2D::Texture2D(const wchar_t* file_path, RenderSystem* render_system)
	: m_render_system(render_system)
{	
	// 텍스처 생성
	DirectX::ScratchImage image_data;
	DirectX::LoadFromWICFile(file_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);
	DirectX::CreateTexture(m_render_system->GetD3DDevice(), image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &m_texture);
	assert(m_texture);

	// 셰이더 리소스 뷰 구조체
	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = image_data.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = static_cast<UINT>(image_data.GetMetadata().mipLevels);
	desc.Texture2D.MostDetailedMip = 0;

	// 셰이더 리소스 뷰 생성
	m_render_system->GetD3DDevice()->CreateShaderResourceView(m_texture.Get(), &desc, &m_shader_resource_view);
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
	m_render_system->GetD3DDevice()->CreateSamplerState(&sampler_desc, &m_sampler_state);
	assert(m_sampler_state);

	// 텍스처 크기 저장
	m_size = Rect(0.0f, 0.0f, static_cast<float>(image_data.GetImages()->width), static_cast<float>(image_data.GetImages()->height));
}

Texture2D::Texture2D(const Rect& size, Texture2D::Type type, RenderSystem* render_system)
	: m_render_system(render_system)
{
	// 텍스처 타입에 따라서 바인딩
	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = size.m_width;
	texture_desc.Height = size.m_height;

	if (type == Texture2D::Type::Normal)
	{
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == Texture2D::Type::RenderTarget)
	{
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == Texture2D::Type::DepthStencil)
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

	m_render_system->GetD3DDevice()->CreateTexture2D(&texture_desc, nullptr, (ID3D11Texture2D**)m_texture.GetAddressOf());
	assert(m_texture);

	if (type == Texture2D::Type::RenderTarget)
	{
		m_render_system->GetD3DDevice()->CreateShaderResourceView(m_texture.Get(), nullptr, &m_shader_resource_view);
		assert(m_shader_resource_view);

		m_render_system->GetD3DDevice()->CreateRenderTargetView(m_texture.Get(), nullptr, &m_render_target_view);
		assert(m_render_target_view);
	}
	else if (type == Texture2D::Type::DepthStencil)
	{
		m_render_system->GetD3DDevice()->CreateDepthStencilView(m_texture.Get(), nullptr, &m_depth_stencil_view);
		assert(m_depth_stencil_view);
	}
	
	m_size = size;
	m_type = type;
}

ID3D11ShaderResourceView* Texture2D::GetShaderResourceView() const
{
	return m_shader_resource_view.Get();
}

ID3D11RenderTargetView* Texture2D::GetRenderTargetView() const
{
	return m_render_target_view.Get();
}

ID3D11DepthStencilView* Texture2D::GetDepthStencilView() const
{
	return m_depth_stencil_view.Get();
}

ID3D11SamplerState* Texture2D::GetSamplerState() const
{
	return m_sampler_state.Get();
}

Rect Texture2D::GetSize() const
{
	return m_size;
}

Texture2D::Type Texture2D::GetType() const
{
	return m_type;
}
