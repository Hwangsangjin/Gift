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
	bool m_locked = true;
	float m_rotation = 0.0f;

	Entity* m_entity = nullptr;

	AudioComponent* m_audio_component = nullptr;
	AudioComponent* m_audio_component2 = nullptr;
};

