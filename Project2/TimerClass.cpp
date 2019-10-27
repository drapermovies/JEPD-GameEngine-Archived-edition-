#include "TimerClass.h"

bool TimerClass::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		return false;
	}

	m_ticksPerMS = (float)m_frequency / 1000.0f;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 current_time = -1;
	float time_difference = -1;

	QueryPerformanceCounter((LARGE_INTEGER*)&current_time);

	time_difference = (float)(current_time - m_startTime);

	m_frameTime = time_difference / m_ticksPerMS;

	m_startTime = current_time;

	return;
}
