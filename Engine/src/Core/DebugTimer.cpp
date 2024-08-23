#include "TimeStep.h"
#include "GLFW/glfw3.h"
#include <iostream>

DebugTimer::DebugTimer(const std::string& description)
	: m_Description(description)
{
	m_StartTime = (float)glfwGetTime();
}

DebugTimer::~DebugTimer()
{
	float endTime = ((float)glfwGetTime() - m_StartTime);

	std::cout << m_Description << ": " << endTime * 1000 << "ms, " << endTime  << "s." << std::endl;
}

