#pragma once

#include "Entity.h"

class Object : public Entity
{
public:
	Object();
	virtual ~Object();

protected:
	virtual void OnCreate() override;
	virtual void OnUpdate(float delta_time) override;

private:
	float m_upward = 0.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
};

