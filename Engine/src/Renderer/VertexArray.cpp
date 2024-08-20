#include "VertexArray.h"
#include "Renderer.h"
#include <iostream>
#include "Platform/OpenGL/OpenGLVertexArray.h"




VertexArray* VertexArray::Create()
{
	switch (Renderer::GetAPI()) {
	case RendererAPI::API::None:
		std::cerr << "No Renderer API selected" << std::endl;
		return nullptr;
		break;
	case RendererAPI::API::OpenGL:
		return new OpenGLVertexArray();
		break;
	}
	return nullptr;
}