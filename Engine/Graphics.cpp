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
#include "Entity.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"

__declspec(align(16))
struct LightData
{
	Vector4 color;
	Vector4 direction;
};

__declspec(align(16))
struct ConstantData
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
	Vector4 camera_position;
	LightData light;
};

Graphics::Graphics(App* app)
	: m_app(app)
{
	m_renderer = std::make_unique<Renderer>();
}

Graphics::~Graphics()
{
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

	for (const auto& m : m_meshes)
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

			m_renderer->SetCullMode(material->GetCullMode());

			material->SetData(&constant_data, sizeof(ConstantData));
			device_context->SetConstantBuffer(material->GetConstantBuffer());

			device_context->SetVertexShader(material->GetVertexShader());
			device_context->SetPixelShader(material->GetPixelShader());

			device_context->SetTexture(&material->m_textures[0], static_cast<UINT>(material->m_textures.size()));

			auto material_slot = mesh->GetMaterialSlot(i);
			device_context->DrawIndexedTriangleList(static_cast<UINT>(material_slot.index_size), static_cast<UINT>(material_slot.start_index), 0);
		}
	}

	swap_chain->Present(true);
}

void Graphics::AddComponent(Component* component)
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
	{
		m_meshes.emplace(c);
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

}

void Graphics::RemoveComponent(Component* component)
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.erase(c);
	else if (auto c = dynamic_cast<CameraComponent*>(component))
		m_cameras.erase(c);
	else if (auto c = dynamic_cast<LightComponent*>(component))
		m_lights.erase(c);
}

Renderer* Graphics::GetRenderer() const
{
	return m_renderer.get();
}
