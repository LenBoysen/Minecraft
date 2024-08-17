#include "Engine.h"


#include"imgui.h"


class Minecraft3DLayer : public Layer {
public:

	Minecraft3DLayer() : Layer("Minecraft3DLayer") {
	}

	virtual void onAttach() override {

	}
	virtual void onDetach() override {

	}
	virtual void onUpdate() override {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

	}
	virtual void onEvent(Event& e) override {
	}
	virtual void onImGuiRender() override {
		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f",ImGui::GetIO().Framerate);
		ImGui::End();
	}

};



class Minecraft : public Application {
public:
	Minecraft() : Application(WindowProps("Minecraft", 1280, 720)) {
		pushLayer(new Minecraft3DLayer);
		getWindow().SetVSync(false);
	}

	~Minecraft() {

	}
private:

};

Application* CreateApplication() {
	return new Minecraft;
}