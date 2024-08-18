#include "Application.h"



#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)


static GLuint ConvertShaderDataTypeToOpenGL(ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:	     return GL_FLOAT;
	case ShaderDataType::Float2:     return GL_FLOAT;
	case ShaderDataType::Float3:     return GL_FLOAT;
	case ShaderDataType::Float4:     return GL_FLOAT;
	case ShaderDataType::Mat3:	     return GL_FLOAT;
	case ShaderDataType::Mat4:	     return GL_FLOAT;
	case ShaderDataType::Int:	     return GL_INT;
	case ShaderDataType::Int2:	     return GL_INT;
	case ShaderDataType::Int3:	     return GL_INT;
	case ShaderDataType::Int4:	     return GL_INT;
	case ShaderDataType::Bool:	     return GL_BOOL;
	}
	return 0;
}


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

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,   0.7f, 0.8f, 0.2f, 1.0f,
		0.5f, -0.5f, 0.0f,    0.7f, 0.1f, 0.8f, 1.0f,
		0.0f, 0.5f, 0.0f,    0.1f, 0.8f, 0.5f, 1.0f,
	};

	m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
	//m_VertexBuffer->Bind();
	
	{

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},

		};

		m_VertexBuffer->SetLayout(layout);

	}

	uint32_t index = 0;
	BufferLayout layout = m_VertexBuffer->GetLayout();
	for (auto& element : layout) { //m_VertexBuffer->GetLayout()
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
			element.GetComponentCount(),
			ConvertShaderDataTypeToOpenGL(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			layout.getStride(),
			(void*)element.Offset);
		index++;
	}


	uint32_t indicies[] = { 0, 1, 2};
	m_IndexBuffer.reset(IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
	//m_IndexBuffer->Bind();


	std::string vertexSrc = R"(
		#version 460 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
		
		out vec4 v_Color;

		void main(){
			v_Color = a_Color;
			gl_Position = vec4(a_Position, 1.0);
		}
	)";
	std::string fragmentSrc = R"(
		#version 460 core
		
		layout(location = 0) out vec4 color;

		in vec4 v_Color;

		void main(){
			//color = vec4(v_Position, 1.0);
			color = v_Color;
		}
	)";


	m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));
}

extern Application* CreateApplication();

void Application::run() {
	
	while (m_Running)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		m_Shader->Bind();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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

