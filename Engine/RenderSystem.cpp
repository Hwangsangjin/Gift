#include "pch.h"
#include "RenderSystem.h"
#include "Engine.h"
#include "Display.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "VertexMesh.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "TerrainComponent.h"

__declspec(align(16))
struct LightData
{
    Vector4 color;
    Vector4 direction;
};

__declspec(align(16))
struct TerrainData
{
    Vector4 size;
    float height_map_size = 0.0f;
};

__declspec(align(16))
struct ConstantData
{
    Matrix4x4 world;
    Matrix4x4 view;
    Matrix4x4 projection;
    Vector4 camera_position;
    LightData light;
    TerrainData terrain;
};

RenderSystem::RenderSystem(Engine* engine)
    : m_engine(engine)
{
    D3D_DRIVER_TYPE driver_types[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT num_driver_types = ARRAYSIZE(driver_types);

    D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };
    UINT num_feature_levels = ARRAYSIZE(feature_levels);

    HRESULT hr = E_FAIL;
    D3D_FEATURE_LEVEL feature_level = {};
    for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
    {
        hr = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &feature_level, &m_immediate_context);
        if (SUCCEEDED(hr))
            break;

        ++driver_type_index;
    }

    assert(m_d3d_device);
    assert(m_immediate_context);

    m_device_context = std::make_shared<DeviceContext>(m_immediate_context.Get(), this);

    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

    InitializeRasterizerState();
    InitializeBlendState();
    CompilePrivateShader();
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Update()
{
    auto swap_chain = m_engine->GetDisplay()->GetSwapChain();

    m_device_context->ClearRenderTarget(swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

    auto client_size = m_engine->GetDisplay()->GetClientSize();
    m_device_context->SetViewport(client_size.m_width, client_size.m_height);

    ConstantData constant_data = {};

    for (const auto& c : m_cameras)
    {
        auto t = c->GetEntity()->GetTransform();
        constant_data.camera_position = t->GetPosition();
        c->SetScreenArea(client_size);
        c->GetViewMatrix(constant_data.view);
        c->GetProjectionMatrix(constant_data.projection);
    }

    for (const auto& l : m_lights)
    {
        auto t = l->GetEntity()->GetTransform();
        Matrix4x4 w;
        t->GetWorldMatrix(w);
        constant_data.light.direction = w.GetZDirection();
        constant_data.light.color = l->GetColor();
    }

    for (const auto& t : m_terrains)
    {
        auto transform = t->GetEntity()->GetTransform();
        transform->GetWorldMatrix(constant_data.world);

        constant_data.terrain.size = t->GetSize();
        constant_data.terrain.height_map_size = t->GetHeightMap()->GetTexture()->GetSize().m_width;

        m_device_context->SetVertexBuffer(t->GetVertexBuffer());
        m_device_context->SetIndexBuffer(t->GetIndexBuffer());

        t->SetConstantData(&constant_data, sizeof(ConstantData));
        m_device_context->SetConstantBuffer(t->GetConstantBuffer());

        m_device_context->SetVertexShader(t->GetVertexShader());
        m_device_context->SetPixelShader(t->GetPixelShader());

        SetCullMode(CullMode::Back);
        SetFillMode(FillMode::Solid);

        Texture2DPtr terrain_textures[3];
        terrain_textures[0] = t->GetHeightMap()->GetTexture();
        terrain_textures[1] = t->GetGroundMap()->GetTexture();
        terrain_textures[2] = t->GetCliffMap()->GetTexture();

        m_device_context->SetTexture(terrain_textures, 3);
        m_device_context->DrawIndexedTriangleList(static_cast<float>(t->GetIndexBuffer()->GetIndexCount()), 0, 0);
    }

    for (const auto& m : m_meshes)
    {
        auto transform = m->GetEntity()->GetTransform();
        transform->GetWorldMatrix(constant_data.world);

        auto mesh = m->GetMesh().get();
        const auto materials = m->GetMaterials();

        m_device_context->SetVertexBuffer(mesh->GetVertexBuffer());
        m_device_context->SetIndexBuffer(mesh->GetIndexBuffer());

        for (auto i = 0; i < mesh->GetMaterialSlotSize(); i++)
        {
            if (i >= materials.size())
                break;

            auto material = materials[i].get();

            SetCullMode(material->GetCullMode());
            SetFillMode(material->GetFillMode());

            material->SetConstantData(&constant_data, sizeof(ConstantData));
            m_device_context->SetConstantBuffer(material->GetConstantBuffer());

            m_device_context->SetVertexShader(material->GetVertexShader());
            m_device_context->SetPixelShader(material->GetPixelShader());

            m_device_context->SetTexture(&material->GetTexture(), static_cast<UINT>(material->GetTextureSize()));

            auto material_slot = mesh->GetMaterialSlot(i);
            m_device_context->DrawIndexedTriangleList(static_cast<UINT>(material_slot.index_size), static_cast<UINT>(material_slot.start_index), 0);
        }
    }

    for (auto& s : m_sprites)
    {
        auto transform = s->GetEntity()->GetTransform();
        transform->GetWorldMatrix(constant_data.world);

        auto mesh = s->GetMesh().get();
        const auto materials = s->GetMaterials();

        m_device_context->SetVertexBuffer(mesh->GetVertexBuffer());
        m_device_context->SetIndexBuffer(mesh->GetIndexBuffer());

        for (auto i = 0; i < mesh->GetMaterialSlotSize(); i++)
        {
            if (i >= materials.size())
                break;

            auto material = materials[i].get();

            SetCullMode(material->GetCullMode());
            SetFillMode(material->GetFillMode());

            material->SetConstantData(&constant_data, sizeof(ConstantData));
            m_device_context->SetConstantBuffer(material->GetConstantBuffer());

            m_device_context->SetVertexShader(material->GetVertexShader());
            m_device_context->SetPixelShader(material->GetPixelShader());

            m_device_context->SetTexture(&material->GetTexture(), static_cast<UINT>(material->GetTextureSize()));

            auto material_slot = mesh->GetMaterialSlot(i);
            m_device_context->DrawIndexedTriangleList(static_cast<UINT>(material_slot.index_size), static_cast<UINT>(material_slot.start_index), 0);
        }
    }
}

void RenderSystem::AddComponent(Component* component)
{
    if (auto c = dynamic_cast<MeshComponent*>(component))
    {
        m_meshes.emplace(c);
    }
    else if (auto c = dynamic_cast<SpriteComponent*>(component))
    {
        m_sprites.emplace(c);
    }
    else if (auto c = dynamic_cast<CameraComponent*>(component))
    {
        if (!m_cameras.size())
            m_cameras.emplace(c);
    }
    else if (auto c = dynamic_cast<LightComponent*>(component))
    {
        if (!m_lights.size())
            m_lights.emplace(c);
    }
    else if (auto c = dynamic_cast<TerrainComponent*>(component))
    {
        if (!m_terrains.size())
            m_terrains.emplace(c);
    }
}

void RenderSystem::RemoveComponent(Component* component)
{
    if (auto c = dynamic_cast<MeshComponent*>(component))
        m_meshes.erase(c);
    else if (auto c = dynamic_cast<SpriteComponent*>(component))
        m_sprites.erase(c);
    else if (auto c = dynamic_cast<CameraComponent*>(component))
        m_cameras.erase(c);
    else if (auto c = dynamic_cast<LightComponent*>(component))
        m_lights.erase(c);
    else if (auto c = dynamic_cast<TerrainComponent*>(component))
        m_terrains.erase(c);
}

SwapChainPtr RenderSystem::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
    return std::make_shared<SwapChain>(hwnd, width, height, this);
}

ConstantBufferPtr RenderSystem::CreateConstantBuffer(void* buffer, UINT buffer_size)
{
    return std::make_shared<ConstantBuffer>(buffer, buffer_size, this);
}

IndexBufferPtr RenderSystem::CreateIndexBuffer(void* index_list, UINT index_count)
{
    return std::make_shared<IndexBuffer>(index_list, index_count, this);
}

VertexBufferPtr RenderSystem::CreateVertexBuffer(void* vertex_list, UINT vertex_size, UINT vertex_count)
{
    return std::make_shared<VertexBuffer>(vertex_list, vertex_size, vertex_count, this);
}

VertexShaderPtr RenderSystem::CreateVertexShader(const wchar_t* file_path)
{
    return std::make_shared<VertexShader>(file_path, this);
}

PixelShaderPtr RenderSystem::CreatePixelShader(const wchar_t* file_path)
{
    return std::make_shared<PixelShader>(file_path, this);
}

Texture2DPtr RenderSystem::CreateTexture(const wchar_t* file_path)
{
    return std::make_shared<Texture2D>(file_path, this);
}

Texture2DPtr RenderSystem::CreateTexture(const Rect& size, Texture2D::Type type)
{
    return std::make_shared<Texture2D>(size, type, this);
}

//void RenderSystem::CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
//{
//    ID3DBlob* error_blob = nullptr;
//    if (FAILED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
//    {
//        if (error_blob)
//        {
//            error_blob->Release();
//            error_blob = nullptr;
//        }
//    }
//
//    assert(m_blob);
//    *shader_byte_code = m_blob->GetBufferPointer();
//    *byte_code_size = m_blob->GetBufferSize();
//}
//
//void RenderSystem::CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
//{
//    ID3DBlob* error_blob = nullptr;
//    if (FAILED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
//    {
//        if (error_blob)
//        {
//            error_blob->Release();
//            error_blob = nullptr;
//        }
//    }
//
//    assert(m_blob);
//    *shader_byte_code = m_blob->GetBufferPointer();
//    *byte_code_size = m_blob->GetBufferSize();
//}
//
//void RenderSystem::ReleaseCompiledShader()
//{
//    if (m_blob)
//    {
//        m_blob->Release();
//        m_blob = nullptr;
//    }
//}

ID3D11Device* RenderSystem::GetD3DDevice() const
{
    return m_d3d_device.Get();
}

ID3D11DeviceContext* RenderSystem::GetImmediateContext() const
{
    return m_immediate_context.Get();
}

IDXGIFactory* RenderSystem::GetDXGIFactory() const
{
    return m_dxgi_factory.Get();
}

DeviceContextPtr RenderSystem::GetDeviceContext() const
{
    return m_device_context;
}

ID3D11BlendState* RenderSystem::GetBlendState() const
{
    return m_alpha_blend.Get();
}

const void* RenderSystem::GetMeshLayoutByteCode() const
{
    return m_mesh_layout_byte_code;
}

size_t RenderSystem::GetMeshLayoutSize() const
{
    return m_mesh_layout_size;
}

void RenderSystem::SetCullMode(const CullMode& cull_mode)
{
    if (cull_mode == CullMode::None)
        m_immediate_context->RSSetState(m_cull_none.Get());
    else if (cull_mode == CullMode::Front)
        m_immediate_context->RSSetState(m_cull_front.Get());
    else if (cull_mode == CullMode::Back)
        m_immediate_context->RSSetState(m_cull_back.Get());
}

void RenderSystem::SetFillMode(const FillMode& fill_mode)
{
    if (fill_mode == FillMode::Wireframe)
        m_immediate_context->RSSetState(m_fill_wireframe.Get());
    else if (fill_mode == FillMode::Solid)
        m_immediate_context->RSSetState(m_fill_solid.Get());
}

void RenderSystem::InitializeRasterizerState()
{
    D3D11_RASTERIZER_DESC desc = {};
    ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
    desc.DepthClipEnable = true;

    desc.CullMode = D3D11_CULL_NONE;
    m_d3d_device->CreateRasterizerState(&desc, m_cull_none.GetAddressOf());

    desc.FillMode = D3D11_FILL_WIREFRAME;
    m_d3d_device->CreateRasterizerState(&desc, m_fill_wireframe.GetAddressOf());

    desc.FillMode = D3D11_FILL_SOLID;
    m_d3d_device->CreateRasterizerState(&desc, m_fill_solid.GetAddressOf());

    desc.CullMode = D3D11_CULL_BACK;
    m_d3d_device->CreateRasterizerState(&desc, m_cull_back.GetAddressOf());

    desc.CullMode = D3D11_CULL_FRONT;
    m_d3d_device->CreateRasterizerState(&desc, m_cull_front.GetAddressOf());
}

void RenderSystem::InitializeBlendState()
{
    D3D11_BLEND_DESC desc = {};
    ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
    desc.RenderTarget[0].BlendEnable = true;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    m_d3d_device->CreateBlendState(&desc, m_alpha_blend.GetAddressOf());
}

void RenderSystem::CompilePrivateShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;

    auto mesh_layout_code = R"(
struct VS_INPUT
{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	return output;
}
    )";

    auto code_size = strlen(mesh_layout_code);

    ::D3DCompile(mesh_layout_code, code_size, "VertexMeshLayout", nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &error_blob);
    assert(blob);

    memcpy(m_mesh_layout_byte_code, blob->GetBufferPointer(), blob->GetBufferSize());
    m_mesh_layout_size = blob->GetBufferSize();
}
