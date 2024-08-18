#include "WindowsInput.h"
#include "Window.h"
#include "WindowsWindow.h"
#include "Application.h"
#include "GLFW/glfw3.h"


Input* Input::s_Instance = new WindowsInput();



bool WindowsInput::isKeyPressedImpl(int keycode)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().getWindow().GetNativeWindow());
	int state = glfwGetKey(window, keycode);
	return  state == GLFW_PRESS || state == GLFW_REPEAT;
}



bool WindowsInput::isMouseButtonPressedImpl(int button) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().getWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return  state == GLFW_PRESS || state == GLFW_REPEAT;
}
std::pair<float, float> WindowsInput::getMousePositionImpl(){
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().getWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);


	return { (float)xpos , (float)ypos};
}
float WindowsInput::getMouseXImpl() {
	auto [x, y] = getMousePositionImpl();
	return x ;
}

float WindowsInput::getMouseYImpl() {
	auto [x, y] = getMousePositionImpl();
	return y;
}

