#include "pch.h"
#include "AudioComponent.h"
#include "Audio.h"
#include "Entity.h"
#include "World.h"
#include "Engine.h"
#include "SoundSystem.h"

AudioComponent::AudioComponent()
{
}

AudioComponent::~AudioComponent()
{
    m_entity->GetWorld()->GetEngine()->GetSoundSystem()->RemoveComponent(this);
}

void AudioComponent::SetAudio(const AudioPtr& audio)
{
    m_audio = audio;
}

void AudioComponent::OnCreate()
{
    m_entity->GetWorld()->GetEngine()->GetSoundSystem()->AddComponent(this);
}
