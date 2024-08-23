#pragma once
#include <string>
#include "Renderer/Shader.h"
#include "glad/glad.h"

class OpenGLShader : public Shader {
public:
	OpenGLShader() {}
	OpenGLShader(const std::string& filePath);
	OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	virtual ~OpenGLShader();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual const std::string GetName() const override;

	void UploadUniformInt(const std::string& name, int value);
	void UploadUniformInt2(const std::string& name, const glm::ivec2& vector);


	void UploadUniformFloat(const std::string& name, float value);
	void UploadUniformFloat2(const std::string& name, const glm::vec2& vector);
	void UploadUniformFloat3(const std::string& name, const glm::vec3& vector);
	void UploadUniformFloat4(const std::string& name, const glm::vec4& vector);
	void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
	void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	virtual void UploadUniformBuffer() override {}
private:
	std::string ReadFile(const std::string& filePath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
private:
	uint32_t m_RendererID;
	std::string m_Name;

};