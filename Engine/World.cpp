#include "pch.h"
#include "World.h"
#include "Entity.h"

World::World(App* app)
	: m_app(app)
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

App* World::GetApp() const
{
	return m_app;
}

void World::RemoveEntity(Entity* entity)
{
	m_entities_to_destroy.emplace(entity);
}
