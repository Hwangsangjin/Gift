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

Texture::~Texture()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	if (m_shader_resource_view)
	{
		m_shader_resource_view->Release();
		m_shader_resource_view = nullptr;
	}

	if (m_sampler_state)
	{
		m_sampler_state->Release();
		m_sampler_state = nullptr;
	}
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() const
{
	return m_shader_resource_view;
}

ID3D11SamplerState* Texture::GetSamplerState() const
{
	return m_sampler_state;
}
