#pragma once
#include <memory>
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "Shader.h"



class Renderer {
public:
	static void Init();
	static void OnWindowResize(uint32_t width, uint32_t height);


	static void BeginScene(Ref<PerspectiveCamera> camera);
	static void EndScene();
	static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));





	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
private:
	struct SceneData {
		glm::mat4 ViewProjectionMatrix;
		glm::mat4 ViewRotationMatrix;
		glm::mat4 ProjectionMatrix;
	};
	static SceneData* s_SceneData;
};