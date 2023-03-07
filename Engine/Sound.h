#pragma once

class Sound
{
public:
	Sound() = default;
	Sound(App* app);
	~Sound();

	void Update();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

private:
	App* m_app = nullptr;

	std::set<AudioComponent*> m_audios;
};

