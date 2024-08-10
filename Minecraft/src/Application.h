#pragma once


#include "Renderer.h"
#include <cassert>
#include <string>
#include <iostream>


class Application {




public:

	~Application();

	Application(uint32_t width, uint32_t height, const std::string& Title);

	Application();

	void run();

private:
	void WindowInit();
private:
	
	GLFWwindow* m_Window = nullptr;
	uint32_t m_WindowWidth;
	uint32_t m_WindowHeight;
	std::string m_WindowTitle;
	bool m_Running = false;
	Renderer m_Renderer;
};
