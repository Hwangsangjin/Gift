#include "pch.h"
#include "Entity.h"
#include "World.h"

Entity::Entity()
{
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

void Entity::Release()
{
	m_world->RemoveEntity(this);
}

World* Entity::GetWorld() const
{
	return m_world;
}
