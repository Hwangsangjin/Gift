#pragma once

class Component
{
public:
	Component();
	virtual ~Component();

	void Release();

	Entity* GetEntity() const;

//protected:
	virtual void OnCreate();

	size_t m_id = 0;
	Entity* m_entity = nullptr;

	friend class Entity;
};

