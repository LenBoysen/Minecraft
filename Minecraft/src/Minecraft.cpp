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
#include "events/MouseEvent.h"
#include "Player.h"

class Minecraft3DLayer : public Layer {
public:

	Minecraft3DLayer() : Layer("Minecraft3DLayer") {
	}

	virtual void onAttach() override {
		
		
		Window& window = Application::Get().getWindow();
		Input::setMousePosition(window.GetWidth() / 2, window.GetHeight() / 2);

		windowMiddlePosition = { window.GetWidth() / 2, window.GetHeight() / 2 };
		Input::setHideCursor(true);

		world.reset(new World());
		world->GenerateWorld();

		m_Player->Spawn(world, { 0,0 });
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
			m_Player->Jump();
			movement.y += 1.0f;
		}
		if (Input::isKeyPressed(EG_KEY_LEFT_SHIFT)) {
			movement.y -= 1.0f;
		}
		movement = glm::length(movement) > 0.0f ? glm::normalize(movement) : movement;


		m_Player->MovePlayer(movement * 10.f, ts.GetSecond(), world);


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

		rotation *= m_Player->GetRotationSpeed() * ts.GetSecond();

		m_Player->RotatePlayer({ (windowMiddlePosition.y - Input::getMousePosition().second) * m_Player->GetRotationSpeed(), (windowMiddlePosition.x - Input::getMousePosition().first) * m_Player->GetRotationSpeed(), 0.0f });


		Input::setMousePosition(windowMiddlePosition.x, windowMiddlePosition.y);
		
	}

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		
		world->ReloadWorld();

		Renderer::BeginScene(m_Player->GetCameraOperator()->GetCamera());
		world->Render();
		Renderer::EndScene();
	}
	virtual void onEvent(Event& e) override {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Minecraft3DLayer::ToggleEscMenu));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Minecraft3DLayer::ReleaseJumpMenu));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(Minecraft3DLayer::MouseClick));

	}
	virtual void onImGuiRender() override {
		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate); 
		ImGui::Checkbox("Escape Menu", &escMenu);
		ImGui::Text("Position: x:%f y:%f z:%f ", m_Player->GetPosition().x, m_Player->GetPosition().y, m_Player->GetPosition().z);
		ImGui::Checkbox("On Ground", m_Player->GetOnGroundPtr());
		ImGui::Text("Distance to Ground: %f", m_Player->GetDistanceToGround(world));
		ImGui::Text("Distance to Ground: %f", 1.0f - 0.3f);
		ImGui::SliderInt("Render distance", &world->GetRenderDistanceRef(), 2, 32);
		//ImGui::SliderFloat("Movement Speed", &movementSpeed, 0.0f, 100.0f);
		ImGui::Text("Speed: %f", glm::length(m_Player->GetMomentum() * glm::vec3(1, 0, 1)));
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
		if (e.GetKeyCode() == EG_KEY_LEFT_CONTROL && e.GetRepeatCount() == 0) {
			m_Player->SetSprinting(true);
		}
		return false;
	}
	bool ReleaseJumpMenu(KeyReleasedEvent& e) {
		if (e.GetKeyCode() == EG_KEY_SPACE) {
			TimeStep::EndTimer("Jump");
		}
		if (e.GetKeyCode() == EG_KEY_LEFT_CONTROL) {
			m_Player->SetSprinting(false);
		}

		return false;
	}
	bool MouseClick(MouseButtonEvent& e) {
		if (escMenu)
			return false;
		std::vector<glm::ivec3> rayBlocks =  world->CastRay(m_Player->GetCameraOperator()->GetCamera()->GetPosition(), m_Player->GetCameraOperator()->GetCamera()->GetRotation());

		if (e.GetMouseButton() == EG_MOUSE_BUTTON_LEFT)
			world->SetBlock(BlockType::None, rayBlocks.end()[-1]);
			
		if (e.GetMouseButton() == EG_MOUSE_BUTTON_RIGHT)
			world->SetBlock(BlockType::Obsidian, rayBlocks.end()[-2]);

		return false;
	}
private: 
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;


	std::shared_ptr<Shader> m_textureShader;
	std::shared_ptr<Shader> m_FlatColorShader;
	std::shared_ptr<VertexArray> m_SquareVA;
	Player* m_Player = Player::Get();
	
	ShaderLibrary& shaderLibrary = Application::Get().getShaderLibrary();
	
	Ref<World> world;

	bool escMenu = false;
	float rotation = 0.0f;
	float scale = 0.1f;
	glm::ivec2 windowMiddlePosition = { 0.0f, 0.0f };
	float speed = 0.0f;

	glm::vec3 squareColor = { 70 / 255.0f, 34 / 255.0f, 118 / 255.0f };
	glm::ivec2 texture = { 2.0f, 2.0f };
	unsigned int index = 0;

};




class Minecraft : public Application {
public:
	Minecraft() : Application(WindowProps("Minecraft", 1920, 1080)) {
		getWindow().SetVSync(false);
		getWindow().SetIcon("Minecraft/assets/textures/logo.png");
		pushLayer(new Minecraft3DLayer);


	}

	~Minecraft() {

	}

private:

};

Application* CreateApplication() {
	return new Minecraft;
}
