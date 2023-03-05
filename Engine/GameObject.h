#pragma once

#include "Entity.h"

class GameObject : public Entity
{
public:
	GameObject();
	virtual ~GameObject();

//protected:
	virtual void OnCreate() override;
	virtual void OnUpdate(float delta_time) override;

private:
	Entity* m_entity = nullptr;
	float m_time = 0.0f;
};

