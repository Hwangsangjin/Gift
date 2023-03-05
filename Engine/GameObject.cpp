#include "pch.h"
#include "GameObject.h"
#include "World.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::OnCreate()
{
	Entity::OnCreate();
	m_entity = GetWorld()->CreateEntity<Entity>();
}

void GameObject::OnUpdate(float delta_time)
{
	Entity::OnUpdate(delta_time);
	m_time += delta_time;

	if (m_entity && m_time >= 3.0f)
	{
		m_entity->Release();
		m_entity = nullptr;
	}
}