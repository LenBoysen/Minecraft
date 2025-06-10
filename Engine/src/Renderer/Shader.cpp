#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <iostream>
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include <cassert>



Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (Renderer::GetAPI()) {
	case RendererAPI::API::None:
		std::cerr << "No Renderer API selected" << std::endl;
		return nullptr;
		break;
	case RendererAPI::API::OpenGL:
		return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		break;
	}
	return nullptr;

}

Ref<Shader> Shader::Create(const std::string& filePath)
{
	switch (Renderer::GetAPI()) {
	case RendererAPI::API::None:
		std::cerr << "No Renderer API selected" << std::endl;
		return nullptr;
		break;
	case RendererAPI::API::OpenGL:
		return std::make_shared<OpenGLShader>(filePath);
		break;
	}
	return nullptr;
}


void ShaderLibrary::Add(const std::string& name, Ref<Shader> shader)
{
	assert(!Exists(name) && "Shader already exists");
	m_Shaders[name] = shader;
	
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
	const std::string& name = shader->GetName();
	Add(name, shader);

}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
	
	auto shader = Shader::Create(filepath);
	auto name = shader->GetName();
	if (Exists(name)) {
		return m_Shaders[name];
	}
	else {
		Add(shader);
		return shader;
	}
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
{
	auto shader = Shader::Create(filepath);
	Add(name, shader);
	return shader;
}

bool ShaderLibrary::Exists(const std::string name)
{
	return m_Shaders.find(name) != m_Shaders.end();
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
	assert(Exists(name) && "Shader not found");

	return m_Shaders[name];

}
