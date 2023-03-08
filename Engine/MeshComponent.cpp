#include "pch.h"
#include "MeshComponent.h"
#include "Entity.h"
#include "World.h"
#include "Engine.h"
#include "RenderSystem.h"

MeshComponent::MeshComponent()
{
}

MeshComponent::~MeshComponent()
{
	m_entity->GetWorld()->GetEngine()->GetRenderSystem()->RemoveComponent(this);
}

const MeshPtr& MeshComponent::GetMesh()
{
	return m_mesh;
}

void MeshComponent::SetMesh(const MeshPtr& mesh)
{
	m_mesh = mesh;
}

void MeshComponent::AddMaterial(const MaterialPtr& material)
{
	m_materials.push_back(material);
}

void MeshComponent::RemoveMaterial(UINT index)
{
	if (index >= m_materials.size())
		return;

	m_materials.erase(m_materials.begin() + index);
}

const std::vector<MaterialPtr>& MeshComponent::GetMaterials()
{
	return m_materials;
}

void MeshComponent::OnCreate()
{
	m_entity->GetWorld()->GetEngine()->GetRenderSystem()->AddComponent(this);
}
