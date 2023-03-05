#include "pch.h"
#include "Graphics.h"
#include "Renderer.h"
#include "App.h"
#include "Display.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

Graphics::Graphics(App* app)
	: m_app(app)
{
	m_renderer = std::make_unique<Renderer>();
}

void Graphics::Update(const ObjectData& object_data)
{
	auto swap_chain = m_app->GetDisplay()->GetSwapChain();

	auto device_context = m_renderer->GetDeviceContext();
	device_context->ClearRenderTarget(swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	auto client_size = m_app->GetDisplay()->GetClientSize();
	device_context->SetViewport(client_size.m_width, client_size.m_height);

	ConstantData constant_data = {};
	constant_data.world.SetIdentity();
	constant_data.view.SetIdentity();
	constant_data.projection.SetIdentity();

	constant_data.world.SetRotationY(0.785f);

	constant_data.view.SetTranslation(Vector3(0.0f, 0.0f, -10.0f));
	constant_data.view.Inverse();

	constant_data.projection.SetPerspectiveProjection(1.3f, static_cast<float>(client_size.m_width) / static_cast<float>(client_size.m_height), 0.01f, 1000.0f);

	device_context->SetVertexBuffer(object_data.mesh->GetVertexBuffer());
	device_context->SetIndexBuffer(object_data.mesh->GetIndexBuffer());

	for (auto i = 0; i < object_data.mesh->GetMaterialSlotSize(); i++)
	{
		object_data.material->SetData(&constant_data, sizeof(ConstantData));
		device_context->SetConstantBuffer(object_data.material->GetConstantBuffer());

		device_context->SetVertexShader(object_data.material->GetVertexShader());
		device_context->SetPixelShader(object_data.material->GetPixelShader());

		device_context->SetTexture(&object_data.material->GetTexture(), static_cast<UINT>(object_data.material->GetTextureSize()));

		auto material_slot = object_data.mesh->GetMaterialSlot(i);
		device_context->DrawIndexedTriangleList(static_cast<UINT>(material_slot.index_size), static_cast<UINT>(material_slot.start_index), 0);
	}

	swap_chain->Present(true);
}

Renderer* Graphics::GetRenderer()
{
	return m_renderer.get();
}
