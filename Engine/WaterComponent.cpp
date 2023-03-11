#include "pch.h"
#include "WaterComponent.h"
#include "Entity.h"
#include "World.h"
#include "Engine.h"
#include "RenderSystem.h"
#include "VertexMesh.h"
#include "ConstantBuffer.h"
#include "Vector3.h"

WaterComponent::WaterComponent()
{
}

WaterComponent::~WaterComponent()
{
    m_entity->GetWorld()->GetEngine()->GetRenderSystem()->RemoveComponent(this);
}

const TexturePtr& WaterComponent::GetWaveHeightMap() const
{
    return m_wave_height_map;
}

void WaterComponent::SetWaveHeightMap(const TexturePtr& wave_height_map)
{
    m_wave_height_map = wave_height_map;
}

void WaterComponent::SetSize(const Vector3& size)
{
    m_size = size;
}

Vector3 WaterComponent::GetSize() const
{
    return m_size;
}

const VertexBufferPtr& WaterComponent::GetVertexBuffer() const
{
    return m_vertex_buffer;
}

const IndexBufferPtr& WaterComponent::GetIndexBuffer() const
{
    return m_index_buffer;
}

const ConstantBufferPtr& WaterComponent::GetConstantBuffer() const
{
    return m_constant_buffer;
}

const VertexShaderPtr& WaterComponent::GetVertexShader() const
{
    return m_vertex_shader;
}

const PixelShaderPtr& WaterComponent::GetPixelShader() const
{
    return m_pixel_shader;
}

void WaterComponent::SetConstantData(void* constant_data, UINT constant_size)
{
    if (!m_constant_buffer)
        m_constant_buffer = m_entity->GetWorld()->GetEngine()->GetRenderSystem()->CreateConstantBuffer(constant_data, constant_size);
    else
        m_constant_buffer->Update(m_entity->GetWorld()->GetEngine()->GetRenderSystem()->GetDeviceContext(), constant_data);
}

void WaterComponent::OnCreate()
{
    m_entity->GetWorld()->GetEngine()->GetRenderSystem()->AddComponent(this);
    GenerateMesh();
}

void WaterComponent::GenerateMesh()
{
    const UINT w = 1024;
    const UINT h = 1024;

    const UINT ww = w - 1;
    const UINT hh = h - 1;

    VertexMesh* vertex_mesh_list = new VertexMesh[w * h];
    UINT* index_list = new UINT[ww * hh * 6];

    auto i = 0;
    for (UINT x = 0; x < w; x++)
    {
        for (UINT y = 0; y < h; y++)
        {
            vertex_mesh_list[x + y * w] =
            {
                Vector3(static_cast<float>(x) / static_cast<float>(ww), 0.0f, static_cast<float>(y) / static_cast<float>(hh)),
                Vector2(static_cast<float>(x) / static_cast<float>(ww), static_cast<float>(y) / static_cast<float>(hh)),
                Vector3(),
                Vector3(),
                Vector3()
            };

            if (x < ww && y < hh)
            {
                index_list[i] = (x)+w * (y + 1);
                index_list[i + 1] = (x)+w * (y);
                index_list[i + 2] = (x + 1) + w * (y);
                index_list[i + 3] = (x + 1) + w * (y);
                index_list[i + 4] = (x + 1) + w * (y + 1);
                index_list[i + 5] = (x)+w * (y + 1);

                i += 6;
            }
        }
    }

    m_vertex_buffer = m_entity->GetWorld()->GetEngine()->GetRenderSystem()->CreateVertexBuffer(vertex_mesh_list, sizeof(VertexMesh), w * h);
    m_index_buffer = m_entity->GetWorld()->GetEngine()->GetRenderSystem()->CreateIndexBuffer(index_list, ww * hh * 6);

    m_vertex_shader = m_entity->GetWorld()->GetEngine()->GetRenderSystem()->CreateVertexShader(L"..\\..\\Assets\\Shaders\\Water.hlsl");
    m_pixel_shader = m_entity->GetWorld()->GetEngine()->GetRenderSystem()->CreatePixelShader(L"..\\..\\Assets\\Shaders\\Water.hlsl");
}
