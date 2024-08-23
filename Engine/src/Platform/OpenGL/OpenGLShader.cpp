#include "eginc.h"
#include "OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include "Core/TimeStep.h"


static GLenum ShaderTypeFromString(const std::string& type) {
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	if (type == "fragment" || type == "pixel")
		return GL_FRAGMENT_SHADER;
	
	//assert
	std::cerr << "Unknown shader type: " << type << std::endl;

}

OpenGLShader::OpenGLShader(const std::string& filePath)
{
	int lastSlash = filePath.find_last_of("/\\") + 1;
	int lastDot = filePath.find_last_of('.');
	int count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
	m_Name = filePath.substr(lastSlash, count);



	Compile(PreProcess(ReadFile(filePath)));
}


OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	: m_Name(name)
{
	std::unordered_map<GLenum, std::string> shaderSources;
	shaderSources[GL_VERTEX_SHADER] = vertexSrc;
	shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
	Compile(shaderSources);
}



OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_RendererID);
}

std::string OpenGLShader::ReadFile(const std::string& filePath)
{
	std::string result;
	std::ifstream in(filePath, std::ios::in | std::ios::binary);
	//assert
	if (in) {
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else {
		std::cerr << "Could not open file: " << filePath << std::endl;
	}
	return result;
}



std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSouces;
	
	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos) {
		size_t eol = source.find_first_of("\r\n", pos);
		//assert eol == npos
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		//assert if type is not fragment pixel or vertex
		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);
		

		shaderSouces[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
	}
	return shaderSouces;
}


void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
	
	GLuint program = glCreateProgram();

	assert(shaderSources.size() <= 2, "increase array size");
	std::array<GLuint, 2> glShaderIDs;
	//std::vector<GLuint> glShaderIDs;
	//glShaderIDs.reserve(shaderSources.size());

	int shaderID = 0;
	for (auto& kv : shaderSources) {
		GLenum type = kv.first;
		const std::string& sources = kv.second;
		GLuint shader = glCreateShader(type);



		const GLchar* source = (const GLchar*)sources.c_str();
		glShaderSource(shader, 1, &source, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			std::cerr << "Shader did not compile" << std::endl
				<< infoLog.data() << std::endl;
			//assert

			return;
		}
		glAttachShader(program, shader);
		//glShaderIDs.push_back(shader);
		glShaderIDs[shaderID++] = shader;
	}


	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(program);
		for(auto& id : glShaderIDs)
			glDeleteShader(id);


		return;
	}

	// Always detach shaders after a successful link.
	for (auto& id : glShaderIDs)
		glDetachShader(program, id);


	m_RendererID = program;
	
}

void OpenGLShader::Bind() const
{
	glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const
{
	glUseProgram(0);
}

const std::string OpenGLShader::GetName() const
{
	return m_Name;
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value)
{
	GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(location, value);
}

void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::ivec2& vector)
{
	GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2i(location, vector.x, vector.y);

}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
{
	GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector)
{
	GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2f(location, vector.x, vector.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector)
{
	GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector)
{
	GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
	GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


