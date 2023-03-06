#include "pch.h"
#include "LightComponent.h"
#include "Entity.h"
#include "App.h"
#include "World.h"
#include "Graphics.h"

LightComponent::LightComponent()
{
}

LightComponent::~LightComponent()
{
    m_entity->GetWorld()->GetApp()->GetGraphics()->RemoveComponent(this);
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
    m_entity->GetWorld()->GetApp()->GetGraphics()->AddComponent(this);
}
