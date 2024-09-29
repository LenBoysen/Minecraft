#include "TimeStep.h"
#include "GLFW/glfw3.h"
#include <iostream>

std::unordered_map<std::string, float>* TimeStep::s_Timers = new std::unordered_map<std::string, float>;

float TimeStep::GetTime(){
	return (float)glfwGetTime();
}

float TimeStep::SetTimer(const std::string& timerKey){
	(*s_Timers)[timerKey] = (float)glfwGetTime();
	return 0.0f;
}

float TimeStep::GetTimer(const std::string& timerKey){
	return (float)glfwGetTime() - (*s_Timers)[timerKey];
}

float TimeStep::ResetTimer(const std::string& timerKey){
	float oldTimer = (*s_Timers)[timerKey];
	(*s_Timers)[timerKey] = (float)glfwGetTime();
	return (*s_Timers)[timerKey] - oldTimer;
}

float TimeStep::EndTimer(const std::string& timerKey){
	float time = (*s_Timers)[timerKey] - (float)glfwGetTime();
	(*s_Timers).erase(timerKey);
	return time;
}

DebugTimer::DebugTimer(const std::string& description)
	: m_Description(description){
	std::cout << "Start DebugTimer" << std::endl;
	m_StartTime = (float)glfwGetTime();
	m_LastTime = m_StartTime;
}

void DebugTimer::AddCounter(const std::string& counterName){
	counter++;
	//std::cout << counter << ") Encountered: " << counterName << " after " << m_LastTime << std::endl;
	//m_LastTime = (float)glfwGetTime();
}


DebugTimer::~DebugTimer(){
	float endTime = ((float)glfwGetTime() - m_StartTime);

	std::cout << m_Description << ": " << endTime * 1000 << "ms, " << endTime  << "s." << std::endl;
	std::cout << "Counted to: " << counter << std::endl;
}
