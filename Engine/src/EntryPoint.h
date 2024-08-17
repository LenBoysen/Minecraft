#include"Application.h"






int main() {
	Application* app = CreateApplication();

	app->run();

	delete app;

	return 0;
}