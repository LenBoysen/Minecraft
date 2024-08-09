#include"Application.h"


void main() {
	Application* app = new Application();

	app->run();

	//app->~Application();
	delete app;
}