#include "pch.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include "SwapChain.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* render_system)
	: m_device_context(device_context)
	, m_render_system(render_system)
{
}

void DeviceContext::ClearRenderTarget(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red, green, blue, alpha };
	auto render_target_view = swap_chain->GetRenderTargetView();
	assert(render_target_view);
	auto depth_stencil_view = swap_chain->GetDepthStencilView();
	assert(depth_stencil_view);
	m_device_context->ClearRenderTargetView(render_target_view, clear_color);
	m_device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	m_device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
}

//void DeviceContext::ClearRenderTarget(const TexturePtr& render_target, float red, float green, float blue, float alpha)
//{
//	if (render_target->GetType() != Texture::Type::RenderTarget)
//		return;
//
//	FLOAT clear_color[] = { red, green, blue, alpha };
//	m_device_context->ClearRenderTargetView(render_target->GetRenderTargetView(), clear_color);
//}

void DeviceContext::ClearDepthStencil(const SwapChainPtr& swap_chain)
{
	m_device_context->ClearDepthStencilView(swap_chain->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

//void DeviceContext::ClearDepthStencil(const TexturePtr& depth_stencil)
//{
//	if (depth_stencil->GetType() != Texture::Type::DepthStencil)
//		return;
//
//	m_device_context->ClearDepthStencilView(depth_stencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
//}
//
//void DeviceContext::SetRenderTarget(const TexturePtr& render_target, const TexturePtr& depth_stencil)
//{
//	if (render_target->GetType() != Texture::Type::RenderTarget)
//		return;
//	if (depth_stencil->GetType() != Texture::Type::DepthStencil)
//		return;
//
//	auto render_target_view = render_target->GetRenderTargetView();
//	assert(render_target_view);
//	auto depth_stencil_view = depth_stencil->GetDepthStencilView();
//	assert(depth_stencil_view);
//	m_device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
//}

void DeviceContext::SetViewport(UINT width, UINT height)
{
	// ����Ʈ ����ü
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// ����Ʈ ����
	m_device_context->RSSetViewports(1, &viewport);
}

void DeviceContext::SetVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	UINT stride = vertex_buffer->GetVertexSize();
	UINT offset = 0;
	auto buffer = vertex_buffer->GetBuffer();
	assert(buffer);
	m_device_context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->GetInputLayout());
}

void DeviceContext::SetVertexShader(const VertexShaderPtr& vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->GetVertexShader(), nullptr, 0);
}

void DeviceContext::SetPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->GetPixelShader(), nullptr, 0);
}

//void DeviceContext::SetTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture_list, UINT texture_size)
//{
//	ID3D11ShaderResourceView* shader_resource_views[32];
//	ID3D11SamplerState* sampler_states[32];
//	for (UINT i = 0; i < texture_size; i++)
//	{
//		shader_resource_views[i] = texture_list[i]->GetShaderResourceView();
//		sampler_states[i] = texture_list[i]->GetSamplerState();
//	}
//
//	assert(shader_resource_views);
//	m_device_context->VSSetShaderResources(0, texture_size, shader_resource_views);
//	assert(sampler_states);
//	m_device_context->VSSetSamplers(0, texture_size, sampler_states);
//}

void DeviceContext::SetTexture(const Texture2DPtr* texture_list, UINT texture_size)
{
	ID3D11ShaderResourceView* shader_resource_views[32];
	ID3D11SamplerState* sampler_states[32];

	for (UINT i = 0; i < texture_size; i++)
	{
		shader_resource_views[i] = texture_list[i]->GetShaderResourceView();
		sampler_states[i] = texture_list[i]->GetSamplerState();
	}

	assert(shader_resource_views);
	m_device_context->VSSetShaderResources(0, texture_size, shader_resource_views);
	m_device_context->PSSetShaderResources(0, texture_size, shader_resource_views);
	assert(sampler_states);
	m_device_context->PSSetSamplers(0, texture_size, sampler_states);
	m_device_context->VSSetSamplers(0, texture_size, sampler_states);
}

void DeviceContext::SetConstantBuffer(const ConstantBufferPtr& constant_buffer)
{
	auto buffer = constant_buffer->GetBuffer();
	assert(buffer);
	m_device_context->VSSetConstantBuffers(0, 1, &buffer);
	m_device_context->PSSetConstantBuffers(0, 1, &buffer);
}

void DeviceContext::SetIndexBuffer(const IndexBufferPtr& index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::DrawTriangleList(UINT vertex_count, UINT start_vertex_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_location);
}

void DeviceContext::DrawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT base_vertex_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count, start_index_location, base_vertex_location);
}

void DeviceContext::DrawTriangleStrip(UINT vertex_count, UINT start_vertex_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_location);
}

ID3D11DeviceContext* DeviceContext::GetDeviceContext()
{
	return m_device_context.Get();
}
