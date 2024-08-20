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
	inline Window& getWindow(){ return *m_Window; }

private:
	bool OnWindowClose(WindowCloseEvent& e);
private:
	float m_LastFrameTime;
	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	LayerStack m_LayerStack;




private:
	static Application* s_Instace;
};

extern Application* CreateApplication();