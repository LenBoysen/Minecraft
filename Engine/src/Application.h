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
#include "Renderer/Buffer.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "glad/glad.h"



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

	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	LayerStack m_LayerStack;

	unsigned int m_VertexArray;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
private:
	static Application* s_Instace;
};

extern Application* CreateApplication();