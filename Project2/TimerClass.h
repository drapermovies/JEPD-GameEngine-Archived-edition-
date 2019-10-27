#pragma once
#include <Windows.h>

class TimerClass
{
public:
	TimerClass() = default;
	TimerClass(const TimerClass&) = default;
	~TimerClass() = default;

	bool Initialize();
	void Frame();

	float GetTime() { return m_frameTime; }
private:
	INT64 m_frequency = -1;
	float m_ticksPerMS = -1.0f;
	INT64 m_startTime = -1;
	float m_frameTime = -1.0f;
};