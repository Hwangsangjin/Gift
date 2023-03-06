#pragma once

#include "App.h"

class Tool : public App
{
public:
	Tool();
	virtual ~Tool() override;

protected:
	virtual void OnCreate() override;
	virtual void OnUpdate(float delta_time) override;

private:
	Entity* m_entity = nullptr;
	float m_rotation = 0.0f;
	bool m_locked = true;
};

