#include "Application.h"

Application::~Application() {

	//m_Renderer.~Renderer();
	
	std::cout << "Application deconstructing.";
}

Application::Application(uint32_t width, uint32_t height, const std::string& Title) : m_WindowWidth(width), m_WindowHeight(height), m_WindowTitle(Title), m_Running(false){

	std::cout << "Application constructing.";
}

Application::Application() : Application(1920, 1080, "Minecraft") {

	std::cout << "Application constructing.";
}

void Application::run() {
	WindowInit();
	m_Renderer.Init(m_Window);
	while (!glfwWindowShouldClose(m_Window))
	{
		m_Renderer.Render();
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}





void Application::WindowInit() {

	if (!glfwInit())
		exit(EXIT_FAILURE);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowTitle.c_str(), NULL, NULL);
	if (!m_Window)
	{
		std::cerr << "GLFWwindow was not created.";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	m_Running = true;
	glfwMakeContextCurrent(m_Window);

	

		

}
