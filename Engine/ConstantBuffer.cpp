#include "pch.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "RenderSystem.h"

ConstantBuffer::ConstantBuffer(void* buffer, UINT buffer_size, RenderSystem* render_system)
    : m_render_system(render_system)
{
    // ���� ����ü
    D3D11_BUFFER_DESC buff_desc = {};
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = buffer_size;
    buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    // ���긮�ҽ� ����ü
    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = buffer;

    // ���� ����
    m_render_system->GetD3DDevice()->CreateBuffer(&buff_desc, &init_data, &m_buffer);
}

void ConstantBuffer::Update(DeviceContextPtr device_context, void* buffer)
{
    device_context->GetDeviceContext()->UpdateSubresource(m_buffer.Get(), NULL, nullptr, buffer, NULL, NULL);
}

ID3D11Buffer* ConstantBuffer::GetBuffer() const
{
    return m_buffer.Get();
}
