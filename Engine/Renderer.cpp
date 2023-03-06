#include "pch.h"
#include "Renderer.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Graphics.h"

Renderer::Renderer()
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
    CompilePrivateShader();
}

SwapChainPtr Renderer::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
    return std::make_shared<SwapChain>(hwnd, width, height, this);
}

ConstantBufferPtr Renderer::CreateConstantBuffer(void* buffer, UINT buffer_size)
{
    return std::make_shared<ConstantBuffer>(buffer, buffer_size, this);
}

IndexBufferPtr Renderer::CreateIndexBuffer(void* index_list, UINT index_count)
{
    return std::make_shared<IndexBuffer>(index_list, index_count, this);
}

VertexBufferPtr Renderer::CreateVertexBuffer(void* vertex_list, UINT vertex_size, UINT vertex_count)
{
    return std::make_shared<VertexBuffer>(vertex_list, vertex_size, vertex_count, this);
}

VertexShaderPtr Renderer::CreateVertexShader(const wchar_t* file_path)
{
    return std::make_shared<VertexShader>(file_path, this);
}

PixelShaderPtr Renderer::CreatePixelShader(const wchar_t* file_path)
{
    return std::make_shared<PixelShader>(file_path, this);
}

Texture2DPtr Renderer::CreateTexture(const wchar_t* file_path)
{
    return std::make_shared<Texture2D>(file_path, this);
}

Texture2DPtr Renderer::CreateTexture(const Rect& size, Texture2D::Type type)
{
    return std::make_shared<Texture2D>(size, type, this);
}

//void Renderer::CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
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
//void Renderer::CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
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
//void Renderer::ReleaseCompiledShader()
//{
//    if (m_blob)
//    {
//        m_blob->Release();
//        m_blob = nullptr;
//    }
//}

ID3D11Device* Renderer::GetD3DDevice() const
{
    return m_d3d_device.Get();
}

ID3D11DeviceContext* Renderer::GetImmediateContext() const
{
    return m_immediate_context.Get();
}

IDXGIFactory* Renderer::GetDXGIFactory() const
{
    return m_dxgi_factory.Get();
}

DeviceContextPtr Renderer::GetDeviceContext() const
{
    return m_device_context;
}

const void* Renderer::GetMeshLayoutByteCode() const
{
    return m_mesh_layout_byte_code;
}

size_t Renderer::GetMeshLayoutSize() const
{
    return m_mesh_layout_size;
}

void Renderer::SetCullMode(bool cull_back)
{
    if (cull_back)
        m_immediate_context->RSSetState(m_cull_back.Get());
    else
        m_immediate_context->RSSetState(m_cull_front.Get());
}

void Renderer::SetFillMode(bool fill_soild)
{
    if (fill_soild)
        m_immediate_context->RSSetState(m_fill_solid.Get());
    else
        m_immediate_context->RSSetState(m_fill_wireframe.Get());
}

void Renderer::InitializeRasterizerState()
{
    D3D11_RASTERIZER_DESC desc = {};
    ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
    desc.DepthClipEnable = true;
    desc.CullMode = D3D11_CULL_NONE;

    desc.FillMode = D3D11_FILL_WIREFRAME;
    m_d3d_device->CreateRasterizerState(&desc, &m_fill_wireframe);

    desc.FillMode = D3D11_FILL_SOLID;
    m_d3d_device->CreateRasterizerState(&desc, &m_fill_solid);

    desc.CullMode = D3D11_CULL_BACK;
    m_d3d_device->CreateRasterizerState(&desc, &m_cull_back);

    desc.CullMode = D3D11_CULL_FRONT;
    m_d3d_device->CreateRasterizerState(&desc, &m_cull_front);
}

void Renderer::CompilePrivateShader()
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

    ::D3DCompile(mesh_layout_code, code_size, "VertexMeshLayoutShader", nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &error_blob);
    assert(blob);

    memcpy(m_mesh_layout_byte_code, blob->GetBufferPointer(), blob->GetBufferSize());
    m_mesh_layout_size = blob->GetBufferSize();
}
