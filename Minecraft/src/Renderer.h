#pragma once
#include "Scene.h".
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "ImGui/ImGuiLayer.h"
#include<map>





class Renderer {
public:
	Renderer() = default;
	~Renderer();

	void Render() const;

	void Init(GLFWwindow* window);

private:

	void RenderScene(const Scene& sc) const;


private:
	GLFWwindow* m_Window;
	ImGuiLayer m_ImGuiLayer;
	Camera m_Camera;
	SceneMgr m_SceneMgr;
	GLuint shaderProgram;
	GLuint VAO, VBO;
};