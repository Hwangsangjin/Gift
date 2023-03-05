#pragma once

class World
{
public:
	World();
	~World();

	template<typename T>
	T* CreateEntity()
	{
		static_assert(std::is_base_of<Entity, T>::value, "T must derive from Entity class.");
		auto id = typeid(T).hash_code();
		auto entity = new T();

		auto entity_ptr = std::unique_ptr<Entity>(entity);
		m_entities[id].emplace(entity, std::move(entity_ptr));
		entity->m_id = id;
		entity->m_world = this;

		entity->OnCreate();

		return entity;
	}

	void Update(float delta_time);

private:
	void RemoveEntity(Entity* entity);

	std::map<size_t, std::map<Entity*, std::unique_ptr<Entity>>> m_entities;
	std::set<Entity*> m_entities_to_destroy;

	friend Entity;
};

