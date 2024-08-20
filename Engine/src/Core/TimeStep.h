#pragma once

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