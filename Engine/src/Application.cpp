#include "Application.h"


#include "Renderer/Renderer.h"
#include "GLFW/glfw3.h"


#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)





Application::~Application() {

	
	std::cout << "Application deconstructing." << std::endl;
}

Application* Application::s_Instace = nullptr;


Application::Application() : Application(WindowProps("Engine", 1280, 700))
{
}


Application::Application(const WindowProps& props) {
	s_Instace = this;
	std::cout << "Application constructing." << std::endl;

	m_Window = std::unique_ptr<Window>(Window::Create(props));
	m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));

	m_ImGuiLayer = new ImGuiLayer();
	pushOverlay(m_ImGuiLayer);



}

extern Application* CreateApplication();

void Application::run() {
	
	while (m_Running)
	{
		float time = (float) glfwGetTime(); //Platform::GetTime()
		TimeStep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;
		for(Layer* layer : m_LayerStack)
			layer->onUpdate(timestep);

		((ImGuiLayer*)m_ImGuiLayer)->begin();
		for (Layer* layer : m_LayerStack)
			layer->onImGuiRender();
		((ImGuiLayer*)m_ImGuiLayer)->end();
		m_Window->onUpdate();
	}
}




void Application::pushLayer(Layer*layer){
	m_LayerStack.PushLayer(layer);
}
void Application::pushOverlay(Layer* layer){
	m_LayerStack.PushOverlay(layer);
}

void Application::onEvent(Event& e){
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
	{
		(*--it)->onEvent(e);
		if (e.Handled)
			break;
	}


}

bool Application::OnWindowClose(WindowCloseEvent & e)
{
	m_Running = false;
	return true;
}

