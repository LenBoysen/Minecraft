#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include <iostream>



VertexBuffer* VertexBuffer::Create(float* verticies, uint32_t size) {
	switch (Renderer::s_RendererAPI) {
	case RendererAPI::None:
		std::cerr << "No Renderer API selected" << std::endl;
		return nullptr;
		break;
	case RendererAPI::OpenGL:
		return new OpenGLVertexBuffer(verticies, size);
		break;
	}
	return nullptr;
}


IndexBuffer* IndexBuffer::Create(uint32_t* indicies, uint32_t count)
{
	switch (Renderer::s_RendererAPI) {
	case RendererAPI::None:
		std::cerr << "No Renderer API selected" << std::endl;
		return nullptr;
	case RendererAPI::OpenGL:
		return new OpenGLIndexBuffer(indicies, count);
		break;

	}
	return nullptr;
}
