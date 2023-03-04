#include "pch.h"
#include "Sound.h"

Sound::Sound()
{
    FMOD::System_Create(&m_system);
    assert(m_system);
    m_system->getVersion(&m_version);
    assert(m_version);
    m_system->init(32, FMOD_INIT_NORMAL, nullptr);
}

Sound::~Sound()
{
    if (m_system)
    {
        m_system->release();
        m_system = nullptr;
    }
}

Sound* Sound::GetInstance()
{
    static Sound instance;
    return &instance;
}

void Sound::CreateSound(const std::string& file_path, const std::string& sound_name, const bool& use_loop)
{
    m_sounds[sound_name] = nullptr;

    auto& sound_ptr = m_sounds[sound_name];

    const int loop_flag = use_loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

    m_system->createSound(file_path.c_str(), loop_flag, 0, &sound_ptr);
}

void Sound::PlaySound(const std::string& sound_name)
{
    const auto& sound_ptr = m_sounds[sound_name];
    auto& channel_ptr = m_channels[sound_ptr];

    bool is_playing = false;
    channel_ptr->isPlaying(&is_playing);

    m_system->playSound(sound_ptr, 0, false, &channel_ptr);
}

void Sound::StopSound(const std::string& sound_name)
{
    const auto& sound_ptr = m_sounds[sound_name];
    auto& channel_ptr = m_channels[sound_ptr];

    bool is_playing = false;
    channel_ptr->isPlaying(&is_playing);

    channel_ptr->stop();
}
