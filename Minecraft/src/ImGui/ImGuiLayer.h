#pragma once
//#include<imgui.h>
#include"../Layer.h"


#include "imgui.h"
#include "../Platform/OpenGL/imgui_impl_opengl3.h"
//#include "../Platform/OpenGL/imgui_impl_glfw.h"


#include "../Application.h"
#include "../events/MouseEvent.h"
#include "../events/KeyEvent.h"
#include "../events/ApplicationEvent.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();
	
	void onAttach();
	void onDetach();
	void onUpdate();
	void onEvent(Event & event);
private:
	bool onMousePress(MouseButtonPressedEvent& e);
	bool onMouseRelease(MouseButtonReleasedEvent& e);
	bool onMouseMoved(MouseMovedEvent& e);
	bool onMouseScroll(MouseScrolledEvent& e);
	bool onKeyPress(KeyPressedEvent& e);
	bool onKeyRelease(KeyReleasedEvent& e);
	bool onKeyTyp(KeyTypedEvent& e);
	bool onWindowResize(WindowResizeEvent& e);
	float m_Time = 0.0f;

};

