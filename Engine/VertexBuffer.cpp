#include "pch.h"
#include "VertexBuffer.h"
#include "Graphics.h"

VertexBuffer::VertexBuffer(void* vertices, UINT vertex_size, UINT vertex_count, void* shader_byte_code, size_t shader_byte_size, Graphics* graphics)
    : m_graphics(graphics)
{
    // 버퍼 구조체
    D3D11_BUFFER_DESC buff_desc = {};
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = vertex_size * vertex_count;
    buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    // 서브리소스 구조체
    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = vertices;

    m_vertex_size = vertex_size;
    m_vertex_count = vertex_count;

    // 버퍼 생성
    m_graphics->GetD3DDevice()->CreateBuffer(&buff_desc, &init_data, &m_buffer);
    assert(m_buffer);

    // 입력 레이아웃 구조체
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT layout_size = ARRAYSIZE(layout);

    // 입력 레이아웃 생성
    m_graphics->GetD3DDevice()->CreateInputLayout(layout, layout_size, shader_byte_code, shader_byte_size, &m_input_layout);
    assert(m_input_layout);
}

VertexBuffer::~VertexBuffer()
{
    if (m_input_layout)
    {
        m_input_layout->Release();
        m_input_layout = nullptr;
    }

    if (m_buffer)
    {
        m_buffer->Release();
        m_buffer = nullptr;
    }
}

ID3D11Buffer* VertexBuffer::GetBuffer() const
{
    return m_buffer;
}

ID3D11InputLayout* VertexBuffer::GetInputLayout() const
{
    return m_input_layout;
}

UINT VertexBuffer::GetVertexSize() const
{
    return m_vertex_size;
}
