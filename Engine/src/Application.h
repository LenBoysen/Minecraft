#pragma once



#include <cassert>
#include <string>
#include <iostream>
#include <memory>

#include "LayerStack.h"
#include "Input.h"
#include "Window.h"
#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Core/TimeStep.h"
#include "Core.h"



class Application {

public:

	~Application();

	Application(const WindowProps& props);
	Application();
	

	void run();
	
	void pushLayer(Layer* layer);

	void pushOverlay(Layer* layer);

	void onEvent(Event& e);


	inline static Application& Get() { return *s_Instace; }
	inline Window& getWindow() { return *m_Window; }
	inline ShaderLibrary& getShaderLibrary() { return shaderLibrary; }

private:
	bool OnWindowClose(WindowCloseEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);
private:
	float m_LastFrameTime;
	Scope<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	bool m_Minimized = false;
	LayerStack m_LayerStack;




protected:
	ShaderLibrary shaderLibrary;

private:
	static Application* s_Instace;
};

extern Application* CreateApplication();