#include "pch.h"
#include "Plane.h"
#include "Engine.h"
#include "Graphics.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"

Plane::Plane()
{
	// ���� �ε�
	m_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\ryan.png");

	Vector3 position[] =
	{
		{ Vector3(-0.5f, -0.5f, -0.5f) },
		{ Vector3(-0.5f, 0.5f, -0.5f) },
		{ Vector3(0.5f, 0.5f, -0.5f) },
		{ Vector3(0.5f, -0.5f, -0.5f) }
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

	// ���� ���̴� ������
	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Engine::GetInstance()->GetGraphics()->CompileVertexShader(L"..\\..\\Assets\\Shaders\\VertexShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);

	// ���� ���̴� ����
	m_vertex_shader = Engine::GetInstance()->GetGraphics()->CreateVertexShader(shader_byte_code, shader_byte_size);
	assert(m_vertex_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// �ȼ� ���̴� ������
	Engine::GetInstance()->GetGraphics()->CompilePixelShader(L"..\\..\\Assets\\Shaders\\PixelShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);

	// �ȼ� ���̴� ����
	m_pixel_shader = Engine::GetInstance()->GetGraphics()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_pixel_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// ���� ���� ����
	m_vertex_buffer = Engine::GetInstance()->GetGraphics()->CreateVertexBuffer(&vertices, sizeof(Vertex), vertex_count, shader_byte_code, (UINT)shader_byte_size);
	assert(m_vertex_buffer);

	// �ε��� ���� ����
	m_index_buffer = Engine::GetInstance()->GetGraphics()->CreateIndexBuffer(&indices, index_count);
	assert(m_index_buffer);

	// ��� ���� ����
	Constant constant;
	m_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&constant, sizeof(Constant));
	assert(m_constant_buffer);
}

Plane::~Plane()
{
}

void Plane::Update()
{
}

void Plane::Render()
{
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetConstantBuffer(m_vertex_shader, m_constant_buffer);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetConstantBuffer(m_pixel_shader, m_constant_buffer);

	// ���̴� ����
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexShader(m_vertex_shader);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetPixelShader(m_pixel_shader);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetTexture(m_pixel_shader, m_texture);

	// ���� ���� ����
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexBuffer(m_vertex_buffer);

	// �ε��� ���� ����
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetIndexBuffer(m_index_buffer);

	// �ﰢ�� �׸���
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->DrawIndexedTriangleList(m_index_buffer->GetIndexCount(), 0, 0);
}
