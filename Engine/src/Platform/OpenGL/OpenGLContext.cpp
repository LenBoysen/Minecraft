#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	//HZ_CORE_ASSERT(windowHandle, "windowHandle is Null");
}

void OpenGLContext::init()
{
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//assert(status);
	//HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl 
			  << "Renderer: " << glGetString(GL_RENDERER) << std::endl
			  << "Version: " << glGetString(GL_VERSION) << std::endl;
}

void OpenGLContext::swapBuffers()
{
	


	glfwSwapBuffers(m_WindowHandle);
}
