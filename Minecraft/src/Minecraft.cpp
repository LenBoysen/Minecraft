#include "Engine.h"

#include "Renderer/OrthographicCamera.h"
#include "Renderer/Renderer.h"

#include"imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"

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
		
		uniform mat4 u_ViewProjection;
		uniform mat4 u_TransformMatrix;
		
		out vec3 v_Position;

		void main(){
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_TransformMatrix * vec4(a_Position, 1.0);
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




		std::string flatColorShaderVertexSrc = R"(
		#version 460 core
		
		layout(location = 0) in vec3 a_Position;
		
		uniform mat4 u_ViewProjection;
		uniform mat4 u_TransformMatrix;


		void main(){
			gl_Position = u_ViewProjection * u_TransformMatrix * vec4(a_Position, 1.0);
		}
	)";
		std::string flatColorShaderFragmentSrc = R"(
		#version 460 core
		
		layout(location = 0) out vec4 color;

		uniform vec3 u_Color;

		void main(){
			color = vec4(u_Color, 1.0f);
		}
	)";


		m_FlatColorShader.reset(Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));


		m_Camera.SetPosition({1.0f, 1.0f, 0.0f});
	}
	virtual void onDetach() override {

	}
	virtual void onUpdate(TimeStep ts) override {

		if (rotate)
			rotation += 100 * ts.GetSecond();

		glm::vec3 movement = glm::vec3(0.0f);

		if (Input::isKeyPressed(EG_KEY_W)) {
			movement.y += 1.0f;
		}
		else if (Input::isKeyPressed(EG_KEY_S)) {
			movement.y -= 1.0f;
		}
		if (Input::isKeyPressed(EG_KEY_A)) {
			movement.x -= 1.0f;
		}
		if (Input::isKeyPressed(EG_KEY_D)) {
			movement.x += 1.0f;
		}
		movement * glm::normalize(movement);
		movement *= movementSpeed * ts.GetSecond();


		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();


		m_Camera.SetPosition(m_Camera.GetPosition() + movement);

		//m_Camera.SetRotation(10*rotation);

		//MaterialRef material = new Material(m_FlatColorShader);
		//MaterialInstanceRef mi = new MaterialInstance(material);
		//material->SetValue("u_Color", redColor);
		//material->SetTexture("u_Albedo", texture);
		//Mesh squareMesh = new Mesh(Vertecies, Indecies); 
		//squareMesh->SetMaterial(material);


		std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", squareColor);

		Renderer::BeginScene(m_Camera);
		for (int x = 0; x < 20; x ++)
		{
			for (int y = 0; y < 20; y++)
			{

				//Renderer::Submit(squareMesh, transform);

				glm::mat4 transform = glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale)), {x*1.7f, y*1.7f, 0.0f}), glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
				Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
		

		Renderer::EndScene();
	}
	virtual void onEvent(Event& e) override {
		
	}
	virtual void onImGuiRender() override {
		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate); 
		ImGui::Checkbox("Rotate", &rotate);
		ImGui::Text("Rotation: %f", rotation);
		ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);
		ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f); 
		ImGui::SliderFloat("Movement Speed", &movementSpeed, 0.0f, 100.0f);
		ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));
		ImGui::End();
	}

private: 
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;


	std::shared_ptr<Shader> m_FlatColorShader;
	std::shared_ptr<VertexArray> m_SquareVA;
	OrthographicCamera m_Camera = OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);

	bool rotate = false;
	float rotation = 0.0f;
	float scale = 0.1f;
	float movementSpeed = 1.5f;

	glm::vec3 squareColor = { 70 / 255.0f, 34 / 255.0f, 118 / 255.0f };
};



class Minecraft : public Application {
public:
	Minecraft() : Application(WindowProps("Minecraft", 1920, 1080)) {
		getWindow().SetVSync(false);
		pushLayer(new Minecraft3DLayer);
	}

	~Minecraft() {

	}
private:

};

Application* CreateApplication() {
	return new Minecraft;
}