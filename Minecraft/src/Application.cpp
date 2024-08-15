#include "Application.h"



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
}


void Application::run() {
	
	m_LayerStack.PushLayer(new RenderLayer);
	m_LayerStack.PushOverlay(new ImGuiLayer);
	//Renderer m_Renderer;
	//m_Renderer.Init(m_Window);
	while (m_Running)
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		for(Layer* layer : m_LayerStack){
			layer->onUpdate();
		}
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
