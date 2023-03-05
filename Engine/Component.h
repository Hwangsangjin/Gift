#pragma once

class Component
{
public:
	Component();
	virtual ~Component();

	void Release();

private:
	size_t m_id = 0;
	Entity* m_entity = nullptr;

	friend class Entity;
};

