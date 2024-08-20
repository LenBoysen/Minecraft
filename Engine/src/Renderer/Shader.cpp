#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <iostream>
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (Renderer::GetAPI()) {
	case RendererAPI::API::None:
		std::cerr << "No Renderer API selected" << std::endl;
		return nullptr;
		break;
	case RendererAPI::API::OpenGL:
		return new OpenGLShader(vertexSrc, fragmentSrc);
		break;
	}
	return nullptr;

}


