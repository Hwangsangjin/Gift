#include "pch.h"
#include "Sprite.h"
#include "Engine.h"
#include "ConstantBuffer.h"

Sprite::Sprite(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	Vector3 position[] =
	{
		{ Vector3(-1.0f, -1.0f, -1.0f) },
		{ Vector3(-1.0f, 1.0f, -1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f) }
	};

	Vector2 texcoord[] =
	{
		{ Vector2(0.0f, 0.0f) },
		{ Vector2(0.0f, 1.0f) },
		{ Vector2(1.0f, 0.0f) },
		{ Vector2(1.0f, 1.0f) }
	};

	Vertex vertices[] =
	{
		{ position[0], texcoord[1] },
		{ position[1], texcoord[0] },
		{ position[2], texcoord[2] },
		{ position[3], texcoord[3] }
	};
	UINT vertex_count = ARRAYSIZE(vertices);

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};
	UINT index_count = ARRAYSIZE(indices);

	// 정점 셰이더 생성
	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Engine::GetInstance()->GetGraphics()->CompileVertexShader(vertex_shader_path, "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_vertex_shader = Engine::GetInstance()->GetGraphics()->CreateVertexShader(shader_byte_code, shader_byte_size);
	assert(m_vertex_shader);

	// 정점 버퍼 생성
	m_vertex_buffer = Engine::GetInstance()->GetGraphics()->CreateVertexBuffer(vertices, sizeof(Vertex), vertex_count, shader_byte_code, (UINT)shader_byte_size);
	assert(m_vertex_buffer);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// 픽셀 셰이더 생성
	Engine::GetInstance()->GetGraphics()->CompilePixelShader(pixel_shader_path, "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_pixel_shader = Engine::GetInstance()->GetGraphics()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_pixel_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// 인덱스 버퍼 생성
	m_index_buffer = Engine::GetInstance()->GetGraphics()->CreateIndexBuffer(indices, index_count);
	assert(m_index_buffer);
}

Sprite::Sprite(const SpritePtr& sprite)
{
	m_vertex_shader = sprite->GetVertexShader();
	assert(m_vertex_shader);
	m_pixel_shader = sprite->GetPixelShader();
	assert(m_pixel_shader);
}

Sprite::~Sprite()
{
}

VertexBufferPtr Sprite::GetVertexBuffer()
{
	return m_vertex_buffer;
}

VertexShaderPtr Sprite::GetVertexShader()
{
	return m_vertex_shader;
}

PixelShaderPtr Sprite::GetPixelShader()
{
	return m_pixel_shader;
}

ConstantBufferPtr Sprite::GetConstantBuffer()
{
	return m_constant_buffer;
}

IndexBufferPtr Sprite::GetIndexBuffer()
{
	return m_index_buffer;
}

TexturePtr Sprite::GetTexture()
{
	return m_vec_textures[0];
}

size_t Sprite::GetTextureSize()
{
	return m_vec_textures.size();
}

void Sprite::AddTexture(const TexturePtr& texture)
{
	m_vec_textures.push_back(texture);
}

void Sprite::RemoveTexture(unsigned int index)
{
	if (index >= m_vec_textures.size())
		return;

	m_vec_textures.erase(m_vec_textures.begin() + index);
}

void Sprite::SetData(void* data, unsigned int size)
{
	if (!m_constant_buffer)
		m_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&data, size);
	else
		m_constant_buffer->Update(Engine::GetInstance()->GetGraphics()->GetDeviceContext(), data);
}

Sprite::CullMode Sprite::GetCullMode()
{
	return m_cull_mode;
}

void Sprite::SetCullMode(CullMode cull_mode)
{
	m_cull_mode = cull_mode;
}

Sprite::FillMode Sprite::GetFillMode()
{
	return m_fill_mode;
}

void Sprite::SetFillMode(FillMode fill_mode)
{
	m_fill_mode = fill_mode;
}
