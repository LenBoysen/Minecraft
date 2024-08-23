#pragma once
#include <string>

class TimeStep {
public:
	TimeStep(float time = 0.0f)
		: m_Time(time){

	}
	float GetSecond() const { return m_Time; }
	float GetMilliseconds() const { return m_Time * 1000.0f; }
private:
	float m_Time;
};

class DebugTimer {
public:
	DebugTimer(const std::string& description);
	~DebugTimer();

private:
	float m_StartTime;
	std::string m_Description;
};



