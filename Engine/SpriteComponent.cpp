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

void SpriteComponent::SetTexture(const TexturePtr& texture)
{
	m_texture = texture;
}

void SpriteComponent::OnCreate()
{
	m_entity->GetWorld()->GetEngine()->GetRenderSystem()->AddComponent(this);
}
