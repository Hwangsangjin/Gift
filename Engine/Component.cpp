#include "pch.h"
#include "Component.h"
#include "Entity.h"

Component::Component()
{
}

Component::~Component()
{
}

void Component::Release()
{
	m_entity->RemoveComponent(m_id);
}

Entity* Component::GetEntity() const
{
	return m_entity;
}

void Component::OnCreate()
{
}
