#include "pch.h"
#include "Entity.h"
#include "World.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "CameraComponent.h"

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

void Entity::CreateComponentInternal(Component* component, size_t id)
{
	auto component_ptr = std::unique_ptr<Component>(component);
	m_components.emplace(id, std::move(component_ptr));
	component->m_id = id;
	component->m_entity = this;

	component->OnCreate();
}

Component* Entity::GetComponentInternal(size_t id)
{
	auto iter = m_components.find(id);
	if (iter == m_components.end())
		return nullptr;
	
	return iter->second.get();
}

void Entity::RemoveComponent(size_t id)
{
	m_components.erase(id);
}

void Entity::Release()
{
	m_world->RemoveEntity(this);
}

InputSystem* Entity::GetInputSystem() const
{
	return m_world->GetEngine()->GetInputSystem();
}

Timer* Entity::GetTimer() const
{
	return m_world->GetEngine()->GetTimer();
}

World* Entity::GetWorld() const
{
	return m_world;
}

TransformComponent* Entity::GetTransform() const
{
	return m_transform;
}
