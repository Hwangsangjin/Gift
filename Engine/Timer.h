#pragma once

class Timer
{
public:
    Timer(Engine* engine);
    ~Timer();

    void Update();

    unsigned int GetFramePerSecond();
    float GetDeltaTime();
    float GetElapsedTime();

    Engine* GetEngine() const;

private:
    unsigned long long m_frequency = 0;
    unsigned long long m_prev_count = 0;

    unsigned int m_frame_count = 0;
    unsigned int m_frame_per_second = 0;
    float m_frame_time = 0.0f;
    float m_delta_time = 0.0f;
    float m_elapsed_time = 0.0f;

    Engine* m_engine = nullptr;
};

