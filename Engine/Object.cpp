#include "pch.h"
#include "Object.h"
#include "World.h"
#include "TransformComponent.h"

Object::Object()
{
}

Object::~Object()
{
}

void Object::OnCreate()
{
	Entity::OnCreate();
	m_entity = GetWorld()->CreateEntity<Entity>();
	m_entity->CreateComponent<Component>();
}

void Object::OnUpdate(float delta_time)
{
	Entity::OnUpdate(delta_time);
	m_elapsed_time += delta_time;

	/*if (m_entity && m_elapsed_time >= 3.0f)
	{
		m_entity->GetComponent<Component>()->Release();
		m_entity->Release();
		m_entity = nullptr;
	}*/

	if (m_entity)
		m_entity->GetTransform()->SetRotation(Vector3(m_elapsed_time, 0.0f, 0.0f));
}