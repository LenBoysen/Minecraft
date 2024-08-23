#include "eginc.h"
#include "Texture.h"
#include "Renderer.h"
#include "RendererAPI.h"
#include <string>
#include <iostream>
#include "Platform/OpenGL/OpenGLTexture.h"


Ref<Texture2D> Texture2D::Create(const std::string& path)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    return nullptr;
	case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
	}

	return nullptr;
}