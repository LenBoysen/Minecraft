#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <iostream>

Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (Renderer::s_RendererAPI) {
	case RendererAPI::None:
		std::cerr << "No Renderer API selected" << std::endl;
		return nullptr;
		break;
	case RendererAPI::OpenGL:
		return new OpenGLShader(vertexSrc, fragmentSrc);
		break;
	}
	return nullptr;

}
