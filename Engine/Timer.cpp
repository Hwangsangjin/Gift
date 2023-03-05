#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_prev_count));
}

Timer::~Timer()
{
}

void Timer::Update()
{
	unsigned long long current_count = 0;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_count));

	m_delta_time = (current_count - m_prev_count) / static_cast<float>(m_frequency);
	m_prev_count = current_count;

	m_frame_count++;
	m_frame_time += m_delta_time;
	m_game_time += m_delta_time;

	if (m_frame_time >= 1.0f)
	{
		m_frame_per_second = static_cast<unsigned int>(m_frame_count / m_frame_time);

		m_frame_time = 0.0f;
		m_frame_count = 0;
	}

#ifdef _DEBUG
	if (m_delta_time >= 1.0f / 60.0f)
		m_delta_time = 1.0f / 60.0f;
#endif
}

unsigned int Timer::GetFramePerSecond()
{
	return m_frame_per_second;
}

float Timer::GetDeltaTime()
{
	return m_delta_time;
}

float Timer::GetGameTime()
{
	return m_game_time;
}
