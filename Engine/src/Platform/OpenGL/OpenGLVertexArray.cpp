#include "OpenGLVertexArray.h"
#include "glad/glad.h"


static GLuint ConvertShaderDataTypeToOpenGL(ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:	     return GL_FLOAT;
	case ShaderDataType::Float2:     return GL_FLOAT;
	case ShaderDataType::Float3:     return GL_FLOAT;
	case ShaderDataType::Float4:     return GL_FLOAT;
	case ShaderDataType::Mat3:	     return GL_FLOAT;
	case ShaderDataType::Mat4:	     return GL_FLOAT;
	case ShaderDataType::Int:	     return GL_INT;
	case ShaderDataType::Int2:	     return GL_INT;
	case ShaderDataType::Int3:	     return GL_INT;
	case ShaderDataType::Int4:	     return GL_INT;
	case ShaderDataType::Bool:	     return GL_BOOL;
	}
	return 0;
}


OpenGLVertexArray::OpenGLVertexArray() {

	glCreateVertexArrays(1, &m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
	glDeleteVertexArrays(1, &m_RendererID);
}



void OpenGLVertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
	//add vertexBuffer has no layout assert TODO
	
	glBindVertexArray(m_RendererID);
	vertexBuffer->Bind();

	uint32_t index = 0;
	BufferLayout layout = vertexBuffer->GetLayout();
	for (auto& element : layout) { //m_VertexBuffer->GetLayout()
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
			element.GetComponentCount(),
			ConvertShaderDataTypeToOpenGL(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			layout.getStride(),
			(void*)element.Offset);
		index++;
	}
	m_VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(m_RendererID);
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
}


