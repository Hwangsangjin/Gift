#pragma once

class Timer
{
public:
    static Timer* GetInstance();

    void Initialize();
    void Update();

    uint32 GetFPS();
    float GetDeltaTime();
    float GetGameTime();

private:
    uint64 m_frequency = 0;
    uint64 m_prev_count = 0;
    float m_delta_time = 0.0f;

    uint32 m_frame_count = 0;
    float m_frame_time = 0.0f;
    float m_game_time = 0.0f;
    uint32 m_fps = 0;
};

