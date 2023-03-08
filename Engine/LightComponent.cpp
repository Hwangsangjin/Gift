#include "pch.h"
#include "LightComponent.h"
#include "Entity.h"
#include "Engine.h"
#include "World.h"
#include "RenderSystem.h"

LightComponent::LightComponent()
{
}

LightComponent::~LightComponent()
{
    m_entity->GetWorld()->GetEngine()->GetRenderSystem()->RemoveComponent(this);
}

Vector4 LightComponent::GetColor() const
{
    return m_color;
}

void LightComponent::SetColor(const Vector4& color)
{
    m_color = color;
}

void LightComponent::OnCreate()
{
    m_entity->GetWorld()->GetEngine()->GetRenderSystem()->AddComponent(this);
}
