#include "Engine.h"


#include"imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/Texture.h"
#include "Renderer/CameraOperator.h"
#include "events/KeyEvent.h"
#include "World.h"

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
		-0.5f, -0.5f, 0.5f,      0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,      1.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,      1.0f, 1.0f,
		-0.5f,  0.5f, 0.5f,      0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,     0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,     1.0f, 1.0f,

		 0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,     0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,     0.0f, 0.0f,

		 -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,

		 -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,
		  0.5f, 0.5f, -0.5f,     1.0f, 1.0f,
		  0.5f, 0.5f,  0.5f,     1.0f, 0.0f,
		 -0.5f, 0.5f,  0.5f,     0.0f, 0.0f,

		 -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,


		};


		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));


		squareVB->SetLayout(BufferLayout{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});


		m_SquareVA->AddVertexBuffer(squareVB);


		std::shared_ptr<IndexBuffer> squareIB;


		uint32_t squareIndicies[] = {

									  4, 6, 7, 4, 5, 6,
									  20, 22, 23, 20, 21, 22,
									  12, 14, 15, 12, 13, 14,
									  8, 10, 11, 8, 9, 10,
									  16, 18, 19, 16, 17, 18,
									  0, 2, 3, 0, 1, 2,
		};


		squareIB.reset(IndexBuffer::Create(squareIndicies, 36));


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


		m_Shader = Shader::Create("PositionShader", vertexSrc, fragmentSrc);

		//Shader::Create("asset/shader/Texture.glsl");


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


		m_FlatColorShader = Shader::Create("FlatColorShader", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);




		std::string textureVertexShader = R"(
		#version 460 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec2 a_TexCoord;
		
		uniform mat4 u_ViewProjection;
		uniform mat4 u_TransformMatrix;
		
		out vec2 v_TexCoord;
		
		
		void main(){
			v_TexCoord = a_TexCoord;
			gl_Position = u_ViewProjection * u_TransformMatrix * vec4(a_Position, 1.0);
		})";
		
		std::string textureFragmentShader = R"(
		#version 460 core
				
		layout(location = 0) out vec4 color;
		
		in vec2 v_TexCoord;
		
		uniform sampler2D u_Texture;
		
		void main(){
			color = texture(u_Texture, v_TexCoord * (1/16.0f) + vec2(15.0f/16.0f, 15.0f/16.0f));
		})";



		//m_textureShader = Shader::Create("assets/shaders/Texture.glsl");
		//auto m_textureShader = shaderLibrary.Load("assets/shaders/Texture.glsl");

		//m_textureShader.reset(Shader::Create(textureVertexShader, textureFragmentShader));

		//m_Texture = Texture2D::Create("assets/textures/terrain.png");

		//std::dynamic_pointer_cast<OpenGLShader>(m_textureShader)->Bind();
		//std::dynamic_pointer_cast<OpenGLShader>(m_textureShader)->UploadUniformInt("u_Texture", 0);

		m_Camera.reset(new PerspectiveCamera(45.0f, 1.6f + 1.6f, 0.9f + 0.9f, 0.01f, 850.0f));

		m_Camera->SetPosition({ 0.0f, 5.0f, 0.0f });
		m_Camera->SetRotation({ -60.0f, 0.0f, 0.0f });

		m_CameraOperator.reset(new CameraOperator(m_Camera));
		Window& window = Application::Get().getWindow();
		Input::setMousePosition(window.GetWidth() / 2, window.GetHeight() / 2);

		windowMiddlePosition = { window.GetWidth() / 2, window.GetHeight() / 2 };
		Input::setHideCursor(true);

		world.reset(new World());
		world->GenerateWorld();
	}
	virtual void onDetach() override {

	}
	virtual void onUpdate(TimeStep ts) override {
		glm::vec3 movement = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);

		if (!escMenu){


		if (Input::isKeyPressed(EG_KEY_W)) {
			movement.z -= 1.0f;
		}
		else if (Input::isKeyPressed(EG_KEY_S)) {
			movement.z += 1.0f;
		}
		if (Input::isKeyPressed(EG_KEY_A)) {
			movement.x -= 1.0f;
		}
		if (Input::isKeyPressed(EG_KEY_D)) {
			movement.x += 1.0f;
		}
		if (Input::isKeyPressed(EG_KEY_SPACE)) {
			movement.y += 1.0f;
		}
		if (Input::isKeyPressed(EG_KEY_LEFT_SHIFT)) {
			movement.y -= 1.0f;
		}
		movement = glm::length(movement) > 0.0f ? glm::normalize(movement) : movement;

		speed = glm::length(movement);

		movement *= movementSpeed * ts.GetSecond();



		if (Input::isKeyPressed(EG_KEY_UP)) {
			rotation.x += 1.0f;
		}
		else if (Input::isKeyPressed(EG_KEY_DOWN)) {
			rotation.x -= 1.0f;
		}
		if (Input::isKeyPressed(EG_KEY_LEFT)) {
			rotation.y += 1.0f;
		}
		if (Input::isKeyPressed(EG_KEY_RIGHT)) {
			rotation.y -= 1.0f;
		}


		rotation = glm::length(rotation) > 0.0f ? glm::normalize(rotation) : rotation;


		rotation *= rotationSpeed * ts.GetSecond();

		//m_Camera->SetPosition(m_Camera->GetPosition() + movement);
		//m_Camera->SetRotation(m_Camera->GetRotation() + rotation);
		m_Camera->SetRotation(m_Camera->GetRotation() + glm::vec3({ (windowMiddlePosition.y - Input::getMousePosition().second) * rotationSpeed, (windowMiddlePosition.x - Input::getMousePosition().first) * rotationSpeed, 0.0f }));

		Input::setMousePosition(windowMiddlePosition.x, windowMiddlePosition.y);
		m_CameraOperator->MoveCamera(movement);
	}
		//MaterialRef material = new Material(m_FlatColorShader);
		//MaterialInstanceRef mi = new MaterialInstance(material);
		//material->SetValue("u_Color", redColor);
		//material->SetTexture("u_Albedo", texture);
		//Mesh squareMesh = new Mesh(Vertecies, Indecies); 
		//squareMesh->SetMaterial(material);

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		

		//m_Texture->Bind();

		Renderer::BeginScene(m_Camera);
		for (int x = 0; x < 20; x ++)
		{
			for (int y = 0; y < 20; y++)
			{

				//Renderer::Submit(squareMesh, transform);

				//glm::mat4 transform = glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0)), { x*1.0f, -2.0f, y*1.0f-20.0f }), glm::radians(0.0f), { 0.0f, 0.0f, 1.0f });
				//Renderer::Submit(m_textureShader, m_SquareVA, transform);
			}
		}
		//Renderer::Submit(m_textureShader, m_SquareVA, glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0)), { 22.0f, -1.0f, -2.0f }), glm::radians(0.0f), { 0.0f, 0.0f, 1.0f }));
		//Renderer::Submit(m_textureShader, m_SquareVA, glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0)), { 21.0f, 0.0f, -3.0f }), glm::radians(0.0f), { 0.0f, 0.0f, 1.0f }));
		//Renderer::Submit(m_textureShader, m_SquareVA, glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0)), { 20.0f, 1.0f, -4.0f }), glm::radians(0.0f), { 0.0f, 0.0f, 1.0f }));
		world->Render();
		Renderer::EndScene();
	}
	virtual void onEvent(Event& e) override {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Minecraft3DLayer::ToggleEscMenu));

		//if (Input::isKeyPressed(EG_KEY_ESCAPE)) {
		//	escMenu ^= true;
		//}
	}
	virtual void onImGuiRender() override {
		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate); 
		ImGui::Checkbox("Escape Menu", &escMenu);
		ImGui::Text("Rotation: %f", rotation);
		ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);
		ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f); 
		ImGui::SliderFloat("Movement Speed", &movementSpeed, 0.0f, 100.0f);
		ImGui::Text("Speed: %f", speed);
		ImGui::SliderInt2("Texture", glm::value_ptr(texture), 0, 15);
		ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));
		ImGui::End();
	}
private:
	bool ToggleEscMenu(KeyPressedEvent& e) {
		if (e.GetKeyCode() == EG_KEY_ESCAPE && e.GetRepeatCount() == 0) {
			Input::setHideCursor(escMenu);
			escMenu ^= true;
			Input::setMousePosition(windowMiddlePosition.x, windowMiddlePosition.y);
		}
		return false;
	}
	
private: 
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;


	std::shared_ptr<Shader> m_textureShader;
	std::shared_ptr<Shader> m_FlatColorShader;
	std::shared_ptr<VertexArray> m_SquareVA;
	Ref<PerspectiveCamera> m_Camera;
	Ref<CameraOperator> m_CameraOperator;

	
	ShaderLibrary& shaderLibrary = Application::Get().getShaderLibrary();
	
	Ref<World> world;

	bool escMenu = false;
	float rotation = 0.0f;
	float scale = 0.1f;
	float movementSpeed = 15.0f;
	float rotationSpeed = 0.05f;
	glm::ivec2 windowMiddlePosition = { 0.0f, 0.0f };
	float speed = 0.0f;

	glm::vec3 squareColor = { 70 / 255.0f, 34 / 255.0f, 118 / 255.0f };
	glm::ivec2 texture = { 2.0f, 2.0f };
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