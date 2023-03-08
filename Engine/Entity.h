#pragma once

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void Release();

	Timer* GetTimer() const;
	World* GetWorld() const;
	InputSystem* GetInputSystem() const;
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
			CreateComponentInternal(component, id);
			return component;
		}

		return nullptr;
	}

	template<typename T>
	T* GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component class.");

		auto id = typeid(T).hash_code();
		return (T*)GetComponentInternal(id);
	}

protected:
	virtual void OnCreate();
	virtual void OnUpdate(float delta_time);

private:
	void CreateComponentInternal(Component* component, size_t id);
	Component* GetComponentInternal(size_t id);
	void RemoveComponent(size_t id);

	size_t m_id = 0;
	World* m_world = nullptr;

	TransformComponent* m_transform = nullptr;
	std::map<size_t, std::unique_ptr<Component>> m_components;

	friend World;
	friend Component;
};

