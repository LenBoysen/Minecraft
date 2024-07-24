#include <GLFW/glfw3.h>
#include <string>
#include <iostream>



class Application {




public:

	~Application() {
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		std::cout << "Application deconstructing.";
	}

	Application(unsigned int width, unsigned int height, std::string Title) : m_WindowWidth(width), m_WindowHeight(height), m_WindowTitle(Title), m_Running(false){

		std::cout << "Application constructing.";
	}

	Application() : Application(1920, 1080, "Minecraft") {

		std::cout << "Application constructing.";
	}

	void run() {
		WindowInit();
		while (!glfwWindowShouldClose(m_Window))
		{

			glfwSwapBuffers(m_Window);
			glfwPollEvents();
		}
	}
private:
	void WindowInit() {

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

		glfwSwapInterval(1);

		

	}

private:
	bool m_Running = false;
	GLFWwindow* m_Window = nullptr;
	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight;
	std::string m_WindowTitle;
};



void main() {

	Application* app = new Application();

	//app->run();

	//app->~Application();
	delete app;
}