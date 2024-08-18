#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include <iostream>
#include "RendererAPI.h"



VertexBuffer* VertexBuffer::Create(float* verticies, uint32_t size) {
	switch (RendererAPI::GetAPI()) {
	case RendererAPI::API::None:
		std::cerr << "No Renderer API selected" << std::endl;
		return nullptr;
		break;
	case RendererAPI::API::OpenGL:
		return new OpenGLVertexBuffer(verticies, size);
		break;
	}
	return nullptr;
}


IndexBuffer* IndexBuffer::Create(uint32_t* indicies, uint32_t count)
{
	switch (RendererAPI::GetAPI()) {
	case RendererAPI::API::None:
		std::cerr << "No Renderer API selected" << std::endl;
		return nullptr;
	case RendererAPI::API::OpenGL:
		return new OpenGLIndexBuffer(indicies, count);
		break;

	}
	return nullptr;
}
