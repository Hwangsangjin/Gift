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
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "Entity.h"

Graphics::Graphics(App* app)
	: m_app(app)
{
	m_renderer = std::make_unique<Renderer>();
}

void Graphics::Update()
{
	auto swap_chain = m_app->GetDisplay()->GetSwapChain();

	auto device_context = m_renderer->GetDeviceContext();
	device_context->ClearRenderTarget(swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	auto client_size = m_app->GetDisplay()->GetClientSize();
	device_context->SetViewport(client_size.m_width, client_size.m_height);

	ConstantData constant_data = {};
	constant_data.view.SetIdentity();
	constant_data.projection.SetIdentity();

	constant_data.view.SetTranslation(Vector3(0.0f, 2.0f, -10.0f));
	constant_data.view.Inverse();

	constant_data.projection.SetPerspectiveProjection(1.3f, static_cast<float>(client_size.m_width) / static_cast<float>(client_size.m_height), 0.01f, 1000.0f);

	for (auto& m : m_meshes)
	{
		auto transform = m->GetEntity()->GetTransform();
		transform->GetWorldMatrix(constant_data.world);

		auto mesh = m->GetMesh().get();
		const auto materials = m->GetMaterials();
			
		device_context->SetVertexBuffer(mesh->GetVertexBuffer());
		device_context->SetIndexBuffer(mesh->GetIndexBuffer());

		for (auto i = 0; i < mesh->GetMaterialSlotSize(); i++)
		{
			if (i >= materials.size())
				break;

			auto material = materials[i].get();

			material->SetData(&constant_data, sizeof(ConstantData));
			device_context->SetConstantBuffer(material->GetConstantBuffer());

			device_context->SetVertexShader(material->GetVertexShader());
			device_context->SetPixelShader(material->GetPixelShader());

			device_context->SetTexture(&material->GetTexture(), static_cast<UINT>(material->GetTextureSize()));

			auto material_slot = mesh->GetMaterialSlot(i);
			device_context->DrawIndexedTriangleList(static_cast<UINT>(material_slot.index_size), static_cast<UINT>(material_slot.start_index), 0);
		}
	}

	swap_chain->Present(true);
}

void Graphics::AddComponent(Component* component)
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.emplace(c);
}

void Graphics::RemoveComponent(Component* component)
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.erase(c);
}

Renderer* Graphics::GetRenderer()
{
	return m_renderer.get();
}
