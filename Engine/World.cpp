#include "pch.h"
#include "World.h"
#include "Entity.h"

World::World(Engine* engine)
	: m_engine(engine)
{
}

World::~World()
{
}

void World::Update(float delta_time)
{
	for (const auto& e : m_entities_to_destroy)
		m_entities[e->m_id].erase(e);

	m_entities_to_destroy.clear();

	for (auto&& [id, entity] : m_entities)
	{
		for (auto&& [ptr, entity] : entity)
		{
			ptr->OnUpdate(delta_time);
		}
	}
}

Engine* World::GetEngine() const
{
	return m_engine;
}

void World::CreateEntityInternal(Entity* entity, size_t id)
{
	auto entity_ptr = std::unique_ptr<Entity>(entity);
	m_entities[id].emplace(entity, std::move(entity_ptr));
	entity->m_id = id;
	entity->m_world = this;

	entity->OnCreate();
}

void World::RemoveEntity(Entity* entity)
{
	m_entities_to_destroy.emplace(entity);
}
