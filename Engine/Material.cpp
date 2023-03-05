#include "pch.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "ResourceManager.h"
#include "App.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Texture.h"

Material::Material(const wchar_t* file_path, ResourceManager* resource_manager)
	: Resource(file_path, resource_manager)
{
	// Á¤Á¡ ¼ÎÀÌ´õ »ý¼º
	m_vertex_shader = m_resource_manager->GetApp()->GetGraphics()->GetRenderer()->CreateVertexShader(file_path);
	assert(m_vertex_shader);

	// ÇÈ¼¿ ¼ÎÀÌ´õ »ý¼º
	m_pixel_shader = m_resource_manager->GetApp()->GetGraphics()->GetRenderer()->CreatePixelShader(file_path);
	assert(m_pixel_shader);
}

Material::Material(const MaterialPtr& material, ResourceManager* resource_manager)
	: Resource(L"", resource_manager)
{
	m_vertex_shader = material->GetVertexShader();
	m_pixel_shader = material->GetPixelShader();
}

Material::~Material()
{
}

VertexShaderPtr Material::GetVertexShader()
{
	return m_vertex_shader;
}

PixelShaderPtr Material::GetPixelShader()
{
	return m_pixel_shader;
}

ConstantBufferPtr Material::GetConstantBuffer()
{
	return m_constant_buffer;
}

Texture2DPtr& Material::GetTexture()
{
	return m_textures[0];
}

size_t Material::GetTextureSize() const
{
	return m_textures.size();
}

void Material::AddTexture(const TexturePtr& texture)
{
	m_textures.push_back(texture->GetTexture());
}

void Material::RemoveTexture(UINT index)
{
	if (index >= m_textures.size())
		return;

	m_textures.erase(m_textures.begin() + index);
}

void Material::SetData(void* data, UINT size)
{
	if (!m_constant_buffer)
		m_constant_buffer = m_resource_manager->GetApp()->GetGraphics()->GetRenderer()->CreateConstantBuffer(data, size);
	else
		m_constant_buffer->Update(m_resource_manager->GetApp()->GetGraphics()->GetRenderer()->GetDeviceContext(), data);
}

Material::CullMode Material::GetCullMode()
{
	return m_cull_mode;
}

void Material::SetCullMode(CullMode cull_mode)
{
	m_cull_mode = cull_mode;
}

Material::FillMode Material::GetFillMode()
{
	return m_fill_mode;
}

void Material::SetFillMode(FillMode fill_mode)
{
	m_fill_mode = fill_mode;
}
