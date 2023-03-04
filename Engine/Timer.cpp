#include "pch.h"
#include "Timer.h"

Timer* Timer::GetInstance()
{
	static Timer instance;
	return &instance;
}

void Timer::CreateTimer()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_prev_count));
}

void Timer::Update()
{
	uint64 currentCount = 0;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	m_delta_time = (currentCount - m_prev_count) / static_cast<float>(m_frequency);
	m_prev_count = currentCount;

	m_frame_count++;
	m_frame_time += m_delta_time;
	m_game_time += m_delta_time;

	if (m_frame_time >= 1.0f)
	{
		m_fps = static_cast<uint32>(m_frame_count / m_frame_time);

		m_frame_time = 0.0f;
		m_frame_count = 0;
	}

#ifdef _DEBUG
	if (m_delta_time >= 1.0f / 60.0f)
		m_delta_time = 1.0f / 60.0f;
#endif
}

uint32 Timer::GetFPS()
{
	return m_fps;
}

float Timer::GetDeltaTime()
{
	return m_delta_time;
}

float Timer::GetGameTime()
{
	return m_game_time;
}
