#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ImGui/ImGuiLayer.h"
#include<map>
#include "Application.h"











class Renderer {
public:
	Renderer();
	~Renderer();

	void Render() const;




private:

	void RenderScene(const Scene& sc) const;


private:
	GLFWwindow* m_Window;
	Camera m_Camera;
	SceneMgr m_SceneMgr;
	GLuint shaderProgram;
	GLuint VAO, VBO;
};

class RenderLayer : public Layer {
public:
	RenderLayer();

	void onAttach();
	void onDetach();
	void onUpdate();
	void onEvent(Event& e);
	void setRenderer(Renderer& renderer);

private:
	Renderer* m_Renderer;
};

