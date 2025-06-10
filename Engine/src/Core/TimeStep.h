#pragma once
#include <string>
#include <unordered_map>

class TimeStep {
public:
	TimeStep(float time = 0.0f)
		: m_Time(time){

	}

	static float GetTime();
	float GetSecond() const { return m_Time; }
	float GetMilliseconds() const { return m_Time * 1000.0f; }
	static float SetTimer(const std::string& timerKey);
	static float GetTimer(const std::string& timerKey);
	static float ResetTimer(const std::string& timerKey);

	static float EndTimer(const std::string& timerKey);
private:
	float m_Time;
	static std::unordered_map<std::string, float>* s_Timers;
};




class DebugTimer {
public:
	DebugTimer(const std::string& description);

	void AddCounter(const std::string& counterName);

	~DebugTimer();

private:
	float m_StartTime;
	float m_LastTime;
	std::string m_Description;
	std::string m_CounterName;
	unsigned long int counter = 0;
};




