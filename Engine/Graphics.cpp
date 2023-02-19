#include "pch.h"
#include "Graphics.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"

Graphics::Graphics()
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
    for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
    {
        hr = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_immediate_context);
        if (SUCCEEDED(hr))
        {
            break;
        }

        ++driver_type_index;
    }

    if (FAILED(hr))
    {
        assert(m_d3d_device);
        assert(m_feature_level);
        assert(m_immediate_context);
        throw std::exception("Device not created successfully");
    }

    m_device_context = std::make_shared<DeviceContext>(m_immediate_context, this);

    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

    InitializeRasterizerState();
}

Graphics::~Graphics()
{
    if (m_dxgi_factory)
    {
        m_dxgi_factory->Release();
        m_dxgi_factory = nullptr;
    }

    if (m_dxgi_adapter)
    {
        m_dxgi_adapter->Release();
        m_dxgi_adapter = nullptr;
    }

    if (m_dxgi_device)
    {
        m_dxgi_device->Release();
        m_dxgi_device = nullptr;
    }

    if (m_immediate_context)
    {
        m_immediate_context->Release();
        m_immediate_context = nullptr;
    }

    if (m_d3d_device)
    {
        m_d3d_device->Release();
        m_d3d_device = nullptr;
    }
}

SwapChainPtr Graphics::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
    return std::make_shared<SwapChain>(hwnd, width, height, this);
}

ConstantBufferPtr Graphics::CreateConstantBuffer(void* buffer, UINT buffer_size)
{
    return std::make_shared<ConstantBuffer>(buffer, buffer_size, this);
}

IndexBufferPtr Graphics::CreateIndexBuffer(void* indices, UINT index_count)
{
    return std::make_shared<IndexBuffer>(indices, index_count, this);
}

VertexBufferPtr Graphics::CreateVertexBuffer(void* vertices, UINT vertex_size, UINT vertex_count, void* shader_byte_code, size_t shader_byte_size)
{
    return std::make_shared<VertexBuffer>(vertices, vertex_size, vertex_count, shader_byte_code, shader_byte_size, this);
}

VertexShaderPtr Graphics::CreateVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
    return std::make_shared<VertexShader>(shader_byte_code, byte_code_size, this);
}

PixelShaderPtr Graphics::CreatePixelShader(const void* shader_byte_code, size_t byte_code_size)
{
    return std::make_shared<PixelShader>(shader_byte_code, byte_code_size, this);
}

void Graphics::CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
    ID3DBlob* error_blob = nullptr;
    if (FAILED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
    {
        if (error_blob)
        {
            error_blob->Release();
            error_blob = nullptr;
        }
    }

    assert(m_blob);
    *shader_byte_code = m_blob->GetBufferPointer();
    *byte_code_size = m_blob->GetBufferSize();
}

void Graphics::CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
    ID3DBlob* error_blob = nullptr;
    if (FAILED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
    {
        if (error_blob)
        {
            error_blob->Release();
            error_blob = nullptr;
        }
    }

    assert(m_blob);
    *shader_byte_code = m_blob->GetBufferPointer();
    *byte_code_size = m_blob->GetBufferSize();
}

void Graphics::ReleaseCompiledShader()
{
    if (m_blob)
    {
        m_blob->Release();
        m_blob = nullptr;
    }
}

ID3D11Device* Graphics::GetD3DDevice() const
{
    return m_d3d_device;
}

ID3D11DeviceContext* Graphics::GetImmediateContext() const
{
    return m_immediate_context;
}

IDXGIFactory* Graphics::GetDXGIFactory() const
{
    return m_dxgi_factory;
}

DeviceContextPtr Graphics::GetDeviceContext() const
{
    return m_device_context;
}

void Graphics::SetRasterizerState(bool cull_front)
{
    if (true)
    {
        m_immediate_context->RSSetState(m_cull_front_state);
    }
    else
    {
        m_immediate_context->RSSetState(m_cull_back_state);
    }
}

void Graphics::InitializeRasterizerState()
{
    D3D11_RASTERIZER_DESC desc = {};
    desc.CullMode = D3D11_CULL_FRONT;
    desc.DepthClipEnable = true;
    desc.FillMode = D3D11_FILL_SOLID;
    m_d3d_device->CreateRasterizerState(&desc, &m_cull_front_state);

    desc.CullMode = D3D11_CULL_FRONT;
    m_d3d_device->CreateRasterizerState(&desc, &m_cull_back_state);

}
