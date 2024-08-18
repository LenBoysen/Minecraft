#pragma once
#include "Renderer/Buffer.h"
#include "glad/glad.h"


class OpenGLVertexBuffer : public VertexBuffer {
public:
	OpenGLVertexBuffer(float* verticies, uint32_t size);
	virtual ~OpenGLVertexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetLayout(const BufferLayout& layout) override { m_BufferLayout = layout; }
	virtual BufferLayout& GetLayout() override { return m_BufferLayout; }

private:
	uint32_t m_RendererID;
	BufferLayout m_BufferLayout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
	OpenGLIndexBuffer(uint32_t* indicies, uint32_t count);
	virtual ~OpenGLIndexBuffer();
	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual inline uint32_t GetCount() const { return m_Count; }
private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};