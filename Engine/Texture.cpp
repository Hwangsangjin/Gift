#include "pch.h"
#include "Texture.h"
#include "Engine.h"

Texture::Texture(const wchar_t* full_path)
	: Resource(full_path)
{
	DirectX::ScratchImage image_data;
	HRESULT hr = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);
	if (SUCCEEDED(hr))
	{
		hr = DirectX::CreateTexture(Engine::GetInstance()->GetGraphics()->GetD3DDevice(), image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &m_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = static_cast<UINT>(image_data.GetMetadata().mipLevels);
		desc.Texture2D.MostDetailedMip = 0;

		Engine::GetInstance()->GetGraphics()->GetD3DDevice()->CreateShaderResourceView(m_texture, &desc, &m_shader_resource_view);
	}
	else
	{
		assert(m_texture);
		throw std::exception("Texture not created successfully");
	}
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
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() const
{
	return m_shader_resource_view;
}
