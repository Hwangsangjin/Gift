#pragma once

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void Release();

	World* GetWorld() const;

protected:
	virtual void OnCreate();
	virtual void OnUpdate(float delta_time);

private:
	size_t m_id = 0;
	World* m_world = nullptr;

	friend World;
};

