#include"Application.h"
#include "imgui.h"


class Minecraft3DLayer : public Layer {
public:

	Minecraft3DLayer() : Layer("Minecraft3DLayer") {
	}

	virtual void onAttach() override{

	}
	virtual void onDetach() override {

	}
	virtual void onUpdate() override {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (Input::isKeyPressed(EG_KEY_F3)) {
			std::cout << "F3 is Press in Minecraft" << std::endl;
		}

	}
	virtual void onEvent(Event& e) override {

	}
	virtual void onImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hallo von Minecraft!");
		ImGui::End();
	}
	
};



class Minecraft : public Application {
public:
	Minecraft() : Application(WindowProps("Minecraft", 1280, 720)){
		pushLayer(new Minecraft3DLayer);
	}
	
	~Minecraft() {

	}
private:

};



int main() {
	Minecraft* minecraft = new Minecraft();

	minecraft->run();

	delete minecraft;

	return 0;
}