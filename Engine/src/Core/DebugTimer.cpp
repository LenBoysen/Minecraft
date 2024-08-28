#include "TimeStep.h"
#include "GLFW/glfw3.h"
#include <iostream>

DebugTimer::DebugTimer(const std::string& description)
	: m_Description(description)
{
	std::cout << "Start DebugTimer" << std::endl;
	m_StartTime = (float)glfwGetTime();
	m_LastTime = m_StartTime;
}

void DebugTimer::AddCounter(const std::string& counterName)
{
	counter++;
	//std::cout << counter << ") Encountered: " << counterName << " after " << m_LastTime << std::endl;
	//m_LastTime = (float)glfwGetTime();
}


DebugTimer::~DebugTimer()
{
	float endTime = ((float)glfwGetTime() - m_StartTime);

	std::cout << m_Description << ": " << endTime * 1000 << "ms, " << endTime  << "s." << std::endl;

	std::cout << "Counted to: " << counter << std::endl;
}
