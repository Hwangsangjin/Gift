#pragma once

#include "Entity.h"

class Object : public Entity
{
public:
	Object();
	virtual ~Object();

//protected:
	virtual void OnCreate() override;
	virtual void OnUpdate(float delta_time) override;

private:
	Entity* m_entity = nullptr;
	float m_elapsed_time = 0.0f;
};

