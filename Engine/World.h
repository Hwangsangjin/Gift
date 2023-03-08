#pragma once

class World
{
public:
	World(Engine* engine);
	~World();

	template<typename T>
	T* CreateEntity()
	{
		static_assert(std::is_base_of<Entity, T>::value, "T must derive from Entity class.");

		auto id = typeid(T).hash_code();
		auto entity = new T();

		CreateEntityInternal(entity, id);

		return entity;
	}

	void Update(float delta_time);

	Engine* GetEngine() const;

private:
	void CreateEntityInternal(Entity* entity, size_t id);
	void RemoveEntity(Entity* entity);

	std::map<size_t, std::map<Entity*, std::unique_ptr<Entity>>> m_entities;
	std::set<Entity*> m_entities_to_destroy;
	Engine* m_engine = nullptr;

	friend Entity;
};

