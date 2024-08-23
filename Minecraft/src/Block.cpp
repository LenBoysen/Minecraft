#include "Block.h"
#include "Application.h"
#include "Platform/OpenGL/OpenGLShader.h"

//s_SquareVA = std::make_shared<VertexArray>(VertexArray::Create());
Block* Block::s_Block = new Block();
const void Block::GenerateVertexArray()
{
	//std::shared_ptr<VertexArray> cubeVA = std::shared_ptr<VertexArray>(VertexArray::Create());
	s_Block->m_CubeVA = std::shared_ptr<VertexArray>(VertexArray::Create());
	
	// Square


	
	
	
	
	float squareVertices[] = {
	-0.5f, -0.5f, 0.5f,      0.0f, 0.0f,    0.0f,
	 0.5f, -0.5f, 0.5f,      1.0f, 0.0f,    0.0f,
	 0.5f,  0.5f, 0.5f,      1.0f, 1.0f,    0.0f,
	-0.5f,  0.5f, 0.5f,      0.0f, 1.0f,    0.0f,

	-0.5f, -0.5f, -0.5f,     1.0f, 0.0f,    1.0f,
	 0.5f, -0.5f, -0.5f,     0.0f, 0.0f,    1.0f,
	 0.5f,  0.5f, -0.5f,     0.0f, 1.0f,    1.0f,
	-0.5f,  0.5f, -0.5f,     1.0f, 1.0f,    1.0f,

	 0.5f, -0.5f, -0.5f,     1.0f, 0.0f,    2.0f,
	 0.5f,  0.5f, -0.5f,     1.0f, 1.0f,    2.0f,
	 0.5f,  0.5f,  0.5f,     0.0f, 1.0f,    2.0f,
	 0.5f, -0.5f,  0.5f,     0.0f, 0.0f,    2.0f,

	 -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    3.0f,
	 -0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    3.0f,
	 -0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    3.0f,
	 -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    3.0f,

	 -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,    4.0f,
	  0.5f, 0.5f, -0.5f,     1.0f, 1.0f,    4.0f,
	  0.5f, 0.5f,  0.5f,     1.0f, 0.0f,    4.0f,
	 -0.5f, 0.5f,  0.5f,     0.0f, 0.0f,    4.0f,

	 -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    5.0f,
	  0.5f, -0.5f, -0.5f,    1.0f, 1.0f,    5.0f,
	  0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    5.0f,
	 -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    5.0f,
	
	
	};
	
	
	s_Block->m_CubeVB = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	
	s_Block->m_CubeVB->SetLayout(BufferLayout{
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float, "a_FaceId" }
		});
	
	
	s_Block->m_CubeVA->AddVertexBuffer(s_Block->m_CubeVB);
	
	
	std::shared_ptr<IndexBuffer> squareIB;
	
	
	uint32_t squareIndicies[] = {
	
								  4, 6, 7, 4, 5, 6,
								  20, 22, 23, 20, 21, 22,
								  12, 14, 15, 12, 13, 14,
								  8, 10, 11, 8, 9, 10,
								  16, 18, 19, 16, 17, 18,
								  0, 2, 3, 0, 1, 2,
	};
	
	s_Block->m_CubeIB = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(squareIndicies, 36));
	
	
	
	s_Block->m_CubeVA->SetIndexBuffer(s_Block->m_CubeIB);

	ShaderLibrary& sl = Application::Get().getShaderLibrary();

	s_Block->m_CubeTextureShader = sl.Load("assets/shaders/Texture.glsl");
	

	s_Block->m_Texture = Texture2D::Create("assets/textures/terrain.png");
	
	s_Block->m_Texture->Bind(0);
	//s_Block->m_CubeTextureShader->UploadUniformInt("u_Texture", 0);
	std::dynamic_pointer_cast<OpenGLShader>(s_Block->m_CubeTextureShader)->Bind();
	std::dynamic_pointer_cast<OpenGLShader>(s_Block->m_CubeTextureShader)->UploadUniformInt("u_Texture", 0);
}