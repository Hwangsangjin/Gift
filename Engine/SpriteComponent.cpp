#include "pch.h"
#include "SpriteComponent.h"
#include "Entity.h"
#include "World.h"
#include "Engine.h"
#include "RenderSystem.h"
#include "Texture.h"

SpriteComponent::SpriteComponent()
{
}

SpriteComponent::~SpriteComponent()
{
	m_entity->GetWorld()->GetEngine()->GetRenderSystem()->RemoveComponent(this);
}

const MeshPtr& SpriteComponent::GetMesh()
{
	return m_mesh;
}

void SpriteComponent::SetMesh(const MeshPtr& mesh)
{
	m_mesh = mesh;
}

void SpriteComponent::AddMaterial(const MaterialPtr& material)
{
	m_materials.push_back(material);
}

void SpriteComponent::RemoveMaterial(UINT index)
{
	if (index >= m_materials.size())
		return;

	m_materials.erase(m_materials.begin() + index);
}

const std::vector<MaterialPtr>& SpriteComponent::GetMaterials()
{
	return m_materials;
}

void SpriteComponent::OnCreate()
{
	m_entity->GetWorld()->GetEngine()->GetRenderSystem()->AddComponent(this);
}
