#include "pch.h"
#include "SoundSystem.h"
#include "Audio.h"
#include "AudioComponent.h"

SoundSystem::SoundSystem(Engine* engine)
	: m_engine(engine)
{
	FMOD::System_Create(&m_system);
	assert(m_system);
	m_system->getVersion(&m_version);
	assert(m_version);
	m_system->init(32, FMOD_INIT_NORMAL, nullptr);
}

SoundSystem::~SoundSystem()
{
	if (m_system)
	{
		m_system->release();
		m_system = nullptr;
	}
}

void SoundSystem::CreateSound(Audio* audio)
{
	char file_path[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, audio->GetFilePath().c_str(), -1, file_path, MAX_PATH, NULL, NULL);

	FMOD::Sound* sound = nullptr;
	m_system->createSound(file_path, FMOD_DEFAULT, 0, &sound);
	assert(sound);
	audio->m_sound = sound;
}

void SoundSystem::PlaySound(AudioComponent* component)
{
	const auto& sound_ptr = component->m_audio->m_sound;
	auto& channel_ptr = m_channels[sound_ptr];

	bool is_playing = false;
	channel_ptr->isPlaying(&is_playing);

	m_system->playSound(sound_ptr, 0, false, &channel_ptr);
}

void SoundSystem::PauseSound(AudioComponent* component)
{
	const auto& audio_ptr = component->m_audio->m_sound;
	auto& channel_ptr = m_channels[audio_ptr];

	bool is_paused = false;
	channel_ptr->getPaused(&is_paused);
	channel_ptr->setPaused(!is_paused);
}

void SoundSystem::StopSound(AudioComponent* component)
{
	const auto& audio_ptr = component->m_audio->m_sound;
	auto& channel_ptr = m_channels[audio_ptr];

	bool is_playing = false;
	channel_ptr->isPlaying(&is_playing);

	channel_ptr->stop();
	channel_ptr = nullptr;
}

void SoundSystem::Update()
{
	for (const auto& a : m_audios)
	{
	}
}

void SoundSystem::AddComponent(Component* component)
{
	if (auto a = dynamic_cast<AudioComponent*>(component))
		m_audios.emplace(a);
}

void SoundSystem::RemoveComponent(Component* component)
{
	if (auto a = dynamic_cast<AudioComponent*>(component))
		m_audios.erase(a);
}
