#include "pch.h"
#include "Entity.h"
#include "World.h"
#include "Component.h"
#include "TransformComponent.h"

Entity::Entity()
{
	m_transform = CreateComponent<TransformComponent>();
}

Entity::~Entity()
{
}

void Entity::OnCreate()
{
}

void Entity::OnUpdate(float delta_time)
{
}

void Entity::RemoveComponent(size_t id)
{
	m_components.erase(id);
}

void Entity::Release()
{
	m_world->RemoveEntity(this);
}

World* Entity::GetWorld() const
{
	return m_world;
}

TransformComponent* Entity::GetTransform() const
{
	return m_transform;
}
