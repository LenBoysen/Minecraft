#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"


RendererAPI* RenderCommand::s_RendererAPI = nullptr;

void RenderCommand::Init() {
	switch (RendererAPI::GetAPI()) {
	case RendererAPI::API::OpenGL:
		RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
		break;
	}
	RenderCommand::s_RendererAPI->Init();
}