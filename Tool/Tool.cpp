#include "pch.h"
#include "Tool.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Display.h"
#include "World.h"
#include "Object.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "MeshComponent.h"
#include "TransformComponent.h"

Tool::Tool()
{
}

Tool::~Tool()
{
}

void Tool::OnCreate()
{
	App::OnCreate();

	auto mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"..\\..\\Assets\\Meshes\\house.obj");
	auto texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\wood.jpg");
	auto material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\DefaultShader.hlsl");
	material->AddTexture(texture);

	m_entity = GetWorld()->CreateEntity<Entity>();
	auto mesh_component = m_entity->CreateComponent<MeshComponent>();
	mesh_component->SetMesh(mesh);
	mesh_component->AddMaterial(material);
	mesh_component->AddMaterial(material);
	mesh_component->AddMaterial(material);
	mesh_component->AddMaterial(material);

	GetWorld()->CreateEntity<Object>();
}

void Tool::OnUpdate(float delta_time)
{
	App::OnUpdate(delta_time);

	m_rotation += 0.785f * delta_time;

	m_entity->GetTransform()->SetRotation(Vector3(0.0f, m_rotation, 0.0f));
}
