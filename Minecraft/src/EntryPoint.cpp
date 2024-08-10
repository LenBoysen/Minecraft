#include"Application.h"


int main() {
	Application* app = new Application();

	app->run();

	//app->~Application();
	delete app;

	return 0;
}