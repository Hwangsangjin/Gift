#include "pch.h"
#include "AudioComponent.h"
#include "Audio.h"
#include "Entity.h"
#include "World.h"
#include "App.h"
#include "Sound.h"

AudioComponent::AudioComponent()
{
}

AudioComponent::~AudioComponent()
{
    m_entity->GetWorld()->GetApp()->GetSound()->RemoveComponent(this);

    if (m_system)
    {
        m_system->release();
        m_system = nullptr;
    }
}

void AudioComponent::SetAudio(const AudioPtr& audio, const std::wstring& audio_name, const bool& use_loop)
{
    FMOD::System_Create(&m_system);
    assert(m_system);
    m_system->getVersion(&m_version);
    assert(m_version);
    m_system->init(32, FMOD_INIT_NORMAL, nullptr);

    m_audios[audio_name] = nullptr;

    auto& audio_ptr = m_audios[audio_name];

    const int loop_flag = use_loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

    int string_length = WideCharToMultiByte(CP_ACP, 0, audio->GetFilePath().c_str(), -1, NULL, 0, NULL, NULL);
    char* file_path = new char[string_length];
    WideCharToMultiByte(CP_ACP, 0, audio->GetFilePath().c_str(), -1, file_path, string_length, NULL, NULL);
    
    m_system->createSound(file_path, loop_flag, 0, &audio_ptr);
}

void AudioComponent::PlayAudio(const wchar_t* audio_name)
{
    const auto& audio_ptr = m_audios[audio_name];
    auto& channel_ptr = m_channels[audio_ptr];

    bool is_playing = false;
    channel_ptr->isPlaying(&is_playing);

    m_system->playSound(audio_ptr, 0, false, &channel_ptr);
}

void AudioComponent::PauseAudio(const wchar_t* audio_name)
{
    const auto& audio_ptr = m_audios[audio_name];
    auto& channel_ptr = m_channels[audio_ptr];

    bool is_paused = false;
    channel_ptr->getPaused(&is_paused);
    channel_ptr->setPaused(!is_paused);
}

void AudioComponent::StopAudio(const wchar_t* audio_name)
{
    const auto& audio_ptr = m_audios[audio_name];
    auto& channel_ptr = m_channels[audio_ptr];

    bool is_playing = false;
    channel_ptr->isPlaying(&is_playing);

    channel_ptr->stop();
    channel_ptr = nullptr;
}

void AudioComponent::OnCreate()
{
    m_entity->GetWorld()->GetApp()->GetSound()->AddComponent(this);
}
