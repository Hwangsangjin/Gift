#pragma once

#include "Component.h"

class AudioComponent : public Component
{
public:
	AudioComponent();
	virtual ~AudioComponent();

	void SetAudio(const AudioPtr& audio, const std::wstring&, const bool& use_loop);
	void PlayAudio(const wchar_t* audio_name);
	void PauseAudio(const wchar_t* audio_name);
	void StopAudio(const wchar_t* audio_name);

protected:
	virtual void OnCreate();

private:
	UINT m_version = 0;
	FMOD::System* m_system = nullptr;
	std::map<std::wstring, FMOD::Sound*> m_audios;
	std::map<FMOD::Sound*, FMOD::Channel*> m_channels;
};

