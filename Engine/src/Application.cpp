#include "Application.h"
#include "imgui.h"



#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::~Application() {

	
	std::cout << "Application deconstructing." << std::endl;
}

Application* Application::s_Instace = nullptr;

Application::Application(){
	s_Instace = this;
	std::cout << "Application constructing." << std::endl;
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));
	m_ImGuiLayer = new ImGuiLayer();
	pushOverlay(m_ImGuiLayer);
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
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		for(Layer* layer : m_LayerStack)
			layer->onUpdate();

		((ImGuiLayer*)m_ImGuiLayer)->begin();
		for (Layer* layer : m_LayerStack)
			layer->onImGuiRender();
		((ImGuiLayer*)m_ImGuiLayer)->end();
		m_Window->onUpdate();
	}
}




//void Application::onEvent(Event & event){
//	EventDispatcher dispatcher(event);
//	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
//	
//	for(auto it = m_LayerStack.end(); it != m_LayerStack.begin()){
//		(*--it).OnEvent(e);
//		if(e.Handled)
//			brake;
//}


void Application::pushLayer(Layer*layer){
	m_LayerStack.PushLayer(layer);
}
void Application::pushOverlay(Layer* layer){
	m_LayerStack.PushOverlay(layer);
}

void Application::onEvent(Event& e)
{
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

