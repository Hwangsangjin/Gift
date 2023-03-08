#pragma once

#include "Component.h"

class AudioComponent : public Component
{
public:
	AudioComponent();
	virtual ~AudioComponent();

	void SetAudio(const AudioPtr& audio);

protected:
	virtual void OnCreate();

private:
	AudioPtr m_audio = nullptr;
	bool m_use_loop = false;

	friend SoundSystem;
};

