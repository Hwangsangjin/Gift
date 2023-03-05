#pragma once

#undef PlaySound

class Sound
{
public:
	static Sound* GetInstance();

	void CreateSound(const std::string& file_path, const std::string& sound_name, const bool& use_loop);
	void PlaySound(const std::string& sound_name);
	void StopSound(const std::string& sound_name);

private:
	Sound();
	~Sound();

	FMOD::System* m_system = nullptr;
	std::map<std::string, FMOD::Sound*> m_sounds;
	std::map<FMOD::Sound*, FMOD::Channel*> m_channels;

	UINT m_version = 0;
};

