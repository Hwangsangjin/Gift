#include "pch.h"
#include "Sound.h"
#include "Audio.h"
#include "AudioComponent.h"

Sound::Sound(App* app)
	: m_app(app)
{
}

Sound::~Sound()
{
}

void Sound::Update()
{
	for (const auto& a : m_audios)
	{
	}
}

void Sound::AddComponent(Component* component)
{
	if (auto a = dynamic_cast<AudioComponent*>(component))
	{
		m_audios.emplace(a);
	}
}

void Sound::RemoveComponent(Component* component)
{
	if (auto a = dynamic_cast<AudioComponent*>(component))
		m_audios.erase(a);
}
