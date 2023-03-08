#pragma once

#undef PlaySound

class SoundSystem
{
public:
	SoundSystem(Engine* engine);
	~SoundSystem();

	void CreateSound(Audio* audio);
	void PlaySound(AudioComponent* component);
	void PauseSound(AudioComponent* component);
	void StopSound(AudioComponent* component);
	void Update();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

private:
	Engine* m_engine = nullptr;

	UINT m_version = 0;
	FMOD::System* m_system = nullptr;
	std::map<FMOD::Sound*, FMOD::Channel*> m_channels;

	std::set<AudioComponent*> m_audios;
};

