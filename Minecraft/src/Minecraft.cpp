#include "Engine.h"


#include"imgui.h"


class Minecraft3DLayer : public Layer {
public:

	Minecraft3DLayer() : Layer("Minecraft3DLayer") {
	}

	virtual void onAttach() override {
		//Triangle

		m_VertexArray.reset(VertexArray::Create());



		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		float vertices[] = {
		-0.5f, -0.5f, 0.0f,   0.7f, 0.8f, 0.2f, 1.0f,
		0.5f, -0.5f, 0.0f,    0.7f, 0.1f, 0.8f, 1.0f,
		0.0f, 0.5f, 0.0f,    0.1f, 0.8f, 0.5f, 1.0f,
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));


		m_VertexBuffer->SetLayout(BufferLayout{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
			});


		m_VertexArray->AddVertexBuffer(m_VertexBuffer);



		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t indicies[] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);



		// Square


		m_SquareVA.reset(VertexArray::Create());


		std::shared_ptr<VertexBuffer> squareVB;


		float squareVertices[] = {
		-0.75f, -0.75f, 0.0f,
		 0.75f, -0.75f, 0.0f,
		 0.75f,  0.75f, 0.0f,
		-0.75f,  0.75f, 0.0f, };


		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));


		squareVB->SetLayout(BufferLayout{
			{ ShaderDataType::Float3, "a_Position" }
			});


		m_SquareVA->AddVertexBuffer(squareVB);


		std::shared_ptr<IndexBuffer> squareIB;


		uint32_t squareIndicies[] = { 0, 2, 3, 0, 1, 2 };


		squareIB.reset(IndexBuffer::Create(squareIndicies, 6));


		m_SquareVA->SetIndexBuffer(squareIB);



		std::string vertexSrc = R"(
		#version 460 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
		
		out vec3 v_Position;

		void main(){
			v_Position = a_Position;
			gl_Position = vec4(a_Position, 1.0);
		}
	)";
		std::string fragmentSrc = R"(
		#version 460 core
		
		layout(location = 0) out vec4 color;

		in vec3 v_Position;

		void main(){
			color = vec4(v_Position*0.5 + 0.5, 1.0);
		}
	)";


		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));




		std::string squraeVertexSrc = R"(
		#version 460 core
		
		layout(location = 0) in vec3 a_Position;
		

		void main(){
			gl_Position = vec4(a_Position, 1.0);
		}
	)";
		std::string squareFragmentSrc = R"(
		#version 460 core
		
		layout(location = 0) out vec4 color;


		void main(){
			color = vec4(0.1, 0.1, 0.7, 1.0);
		}
	)";


		m_SuareShader.reset(Shader::Create(squraeVertexSrc, squareFragmentSrc));
	}
	virtual void onDetach() override {

	}
	virtual void onUpdate() override {
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		Renderer::BeginScene();

		m_SuareShader->Bind();
		Renderer::Submit(m_SquareVA);

		m_Shader->Bind();
		Renderer::Submit(m_VertexArray);

		Renderer::EndScene();
	}
	virtual void onEvent(Event& e) override {
		
	}
	virtual void onImGuiRender() override {
		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f",ImGui::GetIO().Framerate);
		ImGui::End();
	}

private: 
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;


	std::shared_ptr<Shader> m_SuareShader;
	std::shared_ptr<VertexArray> m_SquareVA;
};



class Minecraft : public Application {
public:
	Minecraft() : Application(WindowProps("Minecraft", 1920, 1080)) {
		pushLayer(new Minecraft3DLayer);
	}

	~Minecraft() {

	}
private:

};

Application* CreateApplication() {
	return new Minecraft;
}