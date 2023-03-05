#pragma once

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void Release();

	World* GetWorld() const;
	TransformComponent* GetTransform() const;

	template<typename T>
	T* CreateComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component class.");

		auto component = GetComponent<T>();
		if (!component)
		{
			auto id = typeid(T).hash_code();
			component = new T();

			auto component_ptr = std::unique_ptr<Component>(component);
			m_components.emplace(id, std::move(component_ptr));
			component->m_id = id;
			component->m_entity = this;

			return component;
		}

		return nullptr;
	}

	template<typename T>
	T* GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component class.");

		auto id = typeid(T).hash_code();

		auto iter = m_components.find(id);
		if (iter == m_components.end())
			return nullptr;

		return (T*)iter->second.get();
	}

protected:
	virtual void OnCreate();
	virtual void OnUpdate(float delta_time);

private:
	void RemoveComponent(size_t id);

	size_t m_id = 0;
	World* m_world = nullptr;

	TransformComponent* m_transform = nullptr;
	std::map<size_t, std::unique_ptr<Component>> m_components;

	friend World;
	friend Component;
};

