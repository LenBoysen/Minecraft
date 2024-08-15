#pragma once

#include "Renderer.h"
#include <cassert>
#include <string>
#include <iostream>
#include "Window.h"
#include "LayerStack.h"
#include "events/Event.h"
#include "events/ApplicationEvent.h"


class Application {

public:

	~Application();

	Application(uint32_t width, uint32_t height, const std::string& Title);

	Application();

	void run();
	
	void pushLayer(Layer* layer);

	void pushOverlay(Layer* layer);

	void onEvent(Event& e);


	inline static Application& Get() { return *s_Instace; }
	inline Window& getWindow(){ return *m_Window; }

	inline unsigned int getWindowHeight() const { return m_WindowHeight;}
	inline unsigned int getWindowWidth() const { return m_WindowWidth; }
private:
	bool OnWindowClose(WindowCloseEvent& e);

private:
	
	static Application* s_Instace;
	std::unique_ptr<Window> m_Window;
	unsigned int m_WindowHeight;
	unsigned int m_WindowWidth;
	std::string m_WindowTitle;
	bool m_Running = true;
	//Renderer m_Renderer;
	LayerStack m_LayerStack;
};
