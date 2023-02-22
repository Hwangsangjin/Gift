#include "pch.h"
#include "Material.h"
#include "Engine.h"
#include "ConstantBuffer.h"

Material::Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	// Á¤Á¡ ¼ÎÀÌ´õ »ý¼º
	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Engine::GetInstance()->GetGraphics()->CompileVertexShader(vertex_shader_path, "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_vertex_shader = Engine::GetInstance()->GetGraphics()->CreateVertexShader(shader_byte_code, shader_byte_size);
	assert(m_vertex_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// ÇÈ¼¿ ¼ÎÀÌ´õ »ý¼º
	Engine::GetInstance()->GetGraphics()->CompilePixelShader(pixel_shader_path, "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_pixel_shader = Engine::GetInstance()->GetGraphics()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_pixel_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();
}

Material::Material(const MaterialPtr& material)
{
	m_vertex_shader = material->GetVertexShader();
	assert(m_vertex_shader);
	m_pixel_shader = material->GetPixelShader();
	assert(m_pixel_shader);
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

TexturePtr Material::GetTexture()
{
	return m_vec_textures[0];
}

size_t Material::GetTextureSize()
{
	return m_vec_textures.size();
}

void Material::AddTexture(const TexturePtr& texture)
{
	m_vec_textures.push_back(texture);
}

void Material::RemoveTexture(unsigned int index)
{
	if (index >= m_vec_textures.size())
		return;

	m_vec_textures.erase(m_vec_textures.begin() + index);
}

void Material::SetData(void* data, unsigned int size)
{
	if (!m_constant_buffer)
		m_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&data, size);
	else
		m_constant_buffer->Update(Engine::GetInstance()->GetGraphics()->GetDeviceContext(), data);
			assert(m_constant_buffer);
}

CULL_MODE Material::GetCullMode()
{
	return m_cull_mode;
}

void Material::SetCullMode(CULL_MODE cull_mode)
{
	m_cull_mode = cull_mode;
}
