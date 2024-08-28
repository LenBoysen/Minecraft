#include "Chunk.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Application.h"
#include <memory>
#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include <thread>
#include <future>
#include <tuple>

Chunk::Chunk(glm::ivec2 chunkPosition)
	: m_ChunkPosition(chunkPosition)
{

	//Block::GenerateVertexArray();	s_Block->m_CubeIB = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(squareIndicies, 36));

	

	ShaderLibrary& sl = Application::Get().getShaderLibrary();

	m_ChunkTextureShader = sl.Load("assets/shaders/Texture.glsl");



	m_Texture = Texture2D::Create("assets/textures/terrain.png");

	m_Texture->Bind(0);
	//s_Block->m_CubeTextureShader->UploadUniformInt("u_Texture", 0);
	std::dynamic_pointer_cast<OpenGLShader>(m_ChunkTextureShader)->Bind();
	std::dynamic_pointer_cast<OpenGLShader>(m_ChunkTextureShader)->UploadUniformInt("u_Texture", 0);
	std::dynamic_pointer_cast<OpenGLShader>(m_ChunkTextureShader)->UploadUniformFloat4("u_GrasMultColor", glm::vec4({ 0.35f, 0.78f, 0.29f, 1 }));
	//GenerateIsles();
	for (int z = 0; z < 16; z++) {
		for (int y = 0; y < 512; y++) {
			for (int x = 0; x < 16; x++) {

				if (y == 0)				blocks[x + 256 * y + 16 * z] = BlockType::Bedrock;
				else if (y > 0 && y < 3)	blocks[x + 256 * y + 16 * z] = BlockType::Dirt;
				else if (y == 3)		blocks[x + 256 * y + 16 * z] = BlockType::Grass;
				else					blocks[x + 256 * y + 16 * z] = BlockType::None;

			}
		}
	}
	asyncTask = std::async(std::launch::async, &Chunk::GenereateRenderData, this);

	//SetFutureRet(&ret);
	//ret.wait();


	//sendIsleToRenderer(ret.get());

//	std::thread t1(&Chunk::GenerateIsles, this);
//	t1.join();

}

std::shared_ptr<Chunk> Chunk::Generate(glm::ivec2 chunkPosition)
{
	return std::make_shared<Chunk>(chunkPosition);
}



void Chunk::Render()
{
	
	if (!m_ReadyToRender) {
		if (m_WorldGenDone) {
			sendIsleToRenderer();
			m_ReadyToRender = true;
			m_FallbackExists = true;
		}
		else {
			if (!m_FallbackExists) {
				return;
			}
			else {
			}
		}
	}

	glm::mat4 transfrom = glm::translate(glm::mat4(1.0f), { m_ChunkPosition.x * 16, 0.0f , m_ChunkPosition.y * 16 });
	Renderer::Submit(m_ChunkTextureShader, m_ChunkVA, transfrom);
}

void Chunk::SetBlock(BlockType type, glm::ivec3 pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= 16 || pos.y >= 511 || pos.z >= 16)
		return;
	blocks[pos.x + 256 * pos.y + 16 * pos.z] = type;
	if (m_ReadyToRender) {
		m_ReadyToRender = false;
		m_WorldGenDone = false;
		asyncTask = std::async(std::launch::async, &Chunk::GenereateRenderData, this);
		
	}
	else {
		if (m_WorldGenDone) {
			asyncTask = std::async(std::launch::async, &Chunk::GenereateRenderData, this);
			m_WorldGenDone = false;
		}
		else {
			m_WorldGenHotReload = true;
		}
		
	}

}

void Chunk::GenereateRenderData() {
	unsigned int index = 0;
	glm::vec2 texCoord;
	m_vertecies.reset(new std::vector<float>);
	m_vertecies->reserve(16 * 16 * 512);
	m_indecies.reset(new std::vector<uint32_t>);
	for (glm::ivec3 pos = { 0,0,0 }; pos.z < 16;) {
		for (; pos.y < 512;) {
			for (; pos.x < 16;) {
				BlockType block = GetBlock(pos);
				if (block != BlockType::None) {
					BlockType blockPX = GetBlock(pos + glm::ivec3({ 1,0,0 }));
					BlockType blockNX = GetBlock(pos + glm::ivec3({ -1,0,0 }));
					BlockType blockPY = GetBlock(pos + glm::ivec3({ 0, 1,0 }));
					BlockType blockNY = GetBlock(pos + glm::ivec3({ 0,-1,0 }));
					BlockType blockPZ = GetBlock(pos + glm::ivec3({ 0,0, 1 }));
					BlockType blockNZ = GetBlock(pos + glm::ivec3({ 0,0,-1 }));

					glm::vec2 sideTexCoord;
					glm::vec2 topTexCoord;
					glm::vec2 bottomTexCoord;
					glm::vec4 grasMultColor;
					int texMode;
					switch (block) {
					case BlockType::Stone:		texMode = 0; sideTexCoord = { 1, 0 };  topTexCoord = { 1, 0 };  bottomTexCoord = { 1, 0 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Dirt: 		texMode = 0; sideTexCoord = { 2, 0 };  topTexCoord = { 2, 0 };  bottomTexCoord = { 2, 0 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Grass:		texMode = 1; sideTexCoord = { 3, 0 };  topTexCoord = { 0, 0 };  bottomTexCoord = { 2, 0 };		grasMultColor = { 0.35f, 0.78f, 0.29f, 1 };  break;
					case BlockType::Plank:		texMode = 0; sideTexCoord = { 4, 0 };  topTexCoord = { 4, 0 };  bottomTexCoord = { 4, 0 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Wood: 		texMode = 0; sideTexCoord = { 4, 1 };  topTexCoord = { 4, 1 };  bottomTexCoord = { 4, 1 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Obsidian: 	texMode = 0; sideTexCoord = { 5, 2 };  topTexCoord = { 5, 2 };  bottomTexCoord = { 5, 2 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Bedrock: 	texMode = 0; sideTexCoord = { 1, 1 };  topTexCoord = { 1, 1 };  bottomTexCoord = { 1, 1 };		grasMultColor = { 0, 0, 0, 0 };  break;
					default:					texMode = 0; sideTexCoord = { 15, 1 }; topTexCoord = { 15, 1 };  bottomTexCoord = { 15, 1 };	grasMultColor = { 0, 0, 0, 0 }; break;
					}

					if (blockPX == BlockType::None) {
						texCoord = sideTexCoord;
						m_vertecies->insert(m_vertecies->end(), { (float)pos.x + 1, (float)pos.y,     (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y + 1,
																	(float)pos.x + 1, (float)pos.y + 1, (float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y,
																	(float)pos.x + 1, (float)pos.y,     (float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y + 1,
																	(float)pos.x + 1, (float)pos.y + 1, (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockNX == BlockType::None) {
						texCoord = sideTexCoord;
						m_vertecies->insert(m_vertecies->end(), { (float)pos.x, (float)pos.y,     (float)pos.z,			(float)texCoord.x,		(float)texCoord.y + 1,
																	(float)pos.x, (float)pos.y + 1, (float)pos.z + 1,		(float)texCoord.x + 1,	(float)texCoord.y,
																	(float)pos.x, (float)pos.y,     (float)pos.z + 1,		(float)texCoord.x + 1,	(float)texCoord.y + 1,
																	(float)pos.x, (float)pos.y + 1, (float)pos.z,			(float)texCoord.x,		(float)texCoord.y,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockPY == BlockType::None) {
						texCoord = topTexCoord;
						m_vertecies->insert(m_vertecies->end(), { (float)pos.x,    (float)pos.y + 1, (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y + 1,
																	(float)pos.x + 1,(float)pos.y + 1, (float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y,
																	(float)pos.x,    (float)pos.y + 1, (float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y + 1,
																	(float)pos.x + 1,(float)pos.y + 1,  (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockNY == BlockType::None) {
						texCoord = bottomTexCoord;
						m_vertecies->insert(m_vertecies->end(), { (float)pos.x,     (float)pos.y, (float)pos.z,			(float)texCoord.x,		(float)texCoord.y + 1,
																	(float)pos.x + 1, (float)pos.y, (float)pos.z + 1,		(float)texCoord.x + 1,	(float)texCoord.y,
																	(float)pos.x,     (float)pos.y, (float)pos.z + 1,		(float)texCoord.x + 1,	(float)texCoord.y + 1,
																	(float)pos.x + 1, (float)pos.y, (float)pos.z,			(float)texCoord.x,		(float)texCoord.y,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockPZ == BlockType::None) {
						texCoord = sideTexCoord;
						m_vertecies->insert(m_vertecies->end(), { (float)pos.x, (float)pos.y,			(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y + 1,
																	(float)pos.x + 1, (float)pos.y + 1, (float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y,
																	(float)pos.x + 1, (float)pos.y,     (float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y + 1,
																	(float)pos.x, (float)pos.y + 1,		(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockNZ == BlockType::None) {
						texCoord = sideTexCoord;
						m_vertecies->insert(m_vertecies->end(), { (float)pos.x,		(float)pos.y,     (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y + 1,
																	(float)pos.x + 1,	(float)pos.y + 1, (float)pos.z,		(float)texCoord.x,		(float)texCoord.y,
																	(float)pos.x + 1,	(float)pos.y,     (float)pos.z,		(float)texCoord.x,		(float)texCoord.y + 1,
																	(float)pos.x,		(float)pos.y + 1, (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
				}
				pos.x += 1;
			}
			pos.y += 1;
			pos.x = 0;
		}
		pos.z += 1;
		pos.y = 0;
		pos.x = 0;
	}

	m_layout.reset(new BufferLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" }
		}));
	m_WorldGenDone = true;
	if (m_WorldGenHotReload) {
		m_WorldGenHotReload = false;
		GenereateRenderData();
	}
}

VertexArray* Chunk::GenerateIsles()
{

	//blocks[x + 16 * y + 256 * z]

	//float vertecies[16 * 16 * 512 * 6 * 2 * 3];
	std::vector<float> vertecies;
	std::vector<uint32_t> indecies;
	unsigned int index = 0;
	glm::vec2 texCoord;
	vertecies.reserve(16 * 16 * 512);
	for (glm::ivec3 pos = { 0,0,0 }; pos.z < 16;) {
		for (; pos.y < 512;) {
			for (; pos.x < 16;) {
				BlockType block = GetBlock(pos);
				if (block != BlockType::None) {
					BlockType blockPX = GetBlock(pos + glm::ivec3({ 1,0,0 }));
					BlockType blockNX = GetBlock(pos + glm::ivec3({ -1,0,0 }));
					BlockType blockPY = GetBlock(pos + glm::ivec3({ 0, 1,0 }));
					BlockType blockNY = GetBlock(pos + glm::ivec3({ 0,-1,0 }));
					BlockType blockPZ = GetBlock(pos + glm::ivec3({ 0,0, 1 }));
					BlockType blockNZ = GetBlock(pos + glm::ivec3({ 0,0,-1 }));

					glm::vec2 sideTexCoord;
					glm::vec2 topTexCoord;
					glm::vec2 bottomTexCoord;
					glm::vec4 grasMultColor;
					int texMode;
					switch (block) {
					case BlockType::Stone:		texMode = 0; sideTexCoord = { 1, 0 };  topTexCoord = { 1, 0 };  bottomTexCoord = { 1, 0 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Dirt: 		texMode = 0; sideTexCoord = { 2, 0 };  topTexCoord = { 2, 0 };  bottomTexCoord = { 2, 0 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Grass:		texMode = 1; sideTexCoord = { 3, 0 };  topTexCoord = { 0, 0 };  bottomTexCoord = { 2, 0 };		grasMultColor = { 0.35f, 0.78f, 0.29f, 1 };  break;
					case BlockType::Plank:		texMode = 0; sideTexCoord = { 4, 0 };  topTexCoord = { 4, 0 };  bottomTexCoord = { 4, 0 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Wood: 		texMode = 0; sideTexCoord = { 4, 1 };  topTexCoord = { 4, 1 };  bottomTexCoord = { 4, 1 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Obsidian: 	texMode = 0; sideTexCoord = { 5, 2 };  topTexCoord = { 5, 2 };  bottomTexCoord = { 5, 2 };		grasMultColor = { 0, 0, 0, 0 };  break;
					case BlockType::Bedrock: 	texMode = 0; sideTexCoord = { 1, 1 };  topTexCoord = { 1, 1 };  bottomTexCoord = { 1, 1 };		grasMultColor = { 0, 0, 0, 0 };  break;
					default:					texMode = 0; sideTexCoord = { 15, 1 }; topTexCoord = { 15, 1 };  bottomTexCoord = { 15, 1 };	grasMultColor = { 0, 0, 0, 0 }; break;
					}

					if (blockPX == BlockType::None) {
						texCoord = sideTexCoord;
						vertecies.insert(vertecies.end(), { (float)pos.x + 1, (float)pos.y,     (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y + 1,
															(float)pos.x + 1, (float)pos.y + 1, (float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y,
															(float)pos.x + 1, (float)pos.y,     (float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y + 1,
															(float)pos.x + 1, (float)pos.y + 1, (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y,
							});
						indecies.insert(indecies.end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockNX == BlockType::None) {
						texCoord = sideTexCoord;
						vertecies.insert(vertecies.end(), { (float)pos.x, (float)pos.y,     (float)pos.z,			(float)texCoord.x,		(float)texCoord.y + 1,
															(float)pos.x, (float)pos.y + 1, (float)pos.z + 1,		(float)texCoord.x + 1,	(float)texCoord.y,
															(float)pos.x, (float)pos.y,     (float)pos.z + 1,		(float)texCoord.x + 1,	(float)texCoord.y + 1,
															(float)pos.x, (float)pos.y + 1, (float)pos.z,			(float)texCoord.x,		(float)texCoord.y, 
							});
						indecies.insert(indecies.end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockPY == BlockType::None) {
						texCoord = topTexCoord;
						vertecies.insert(vertecies.end(), { (float)pos.x,    (float)pos.y + 1, (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y + 1,
															(float)pos.x + 1,(float)pos.y + 1, (float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y,
															(float)pos.x,    (float)pos.y + 1, (float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y + 1,
															(float)pos.x + 1,(float)pos.y + 1,  (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y,
							});
						indecies.insert(indecies.end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockNY == BlockType::None) {
						texCoord = bottomTexCoord;
						vertecies.insert(vertecies.end(), { (float)pos.x,     (float)pos.y, (float)pos.z,			(float)texCoord.x,		(float)texCoord.y + 1,	
															(float)pos.x + 1, (float)pos.y, (float)pos.z + 1,		(float)texCoord.x + 1,	(float)texCoord.y,		
															(float)pos.x,     (float)pos.y, (float)pos.z + 1,		(float)texCoord.x + 1,	(float)texCoord.y + 1,	
															(float)pos.x + 1, (float)pos.y, (float)pos.z,			(float)texCoord.x,		(float)texCoord.y,		
							});
						indecies.insert(indecies.end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockPZ == BlockType::None) {
						texCoord = sideTexCoord;
						vertecies.insert(vertecies.end(), { (float)pos.x, (float)pos.y,			(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y + 1,
															(float)pos.x + 1, (float)pos.y + 1, (float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y,
															(float)pos.x + 1, (float)pos.y,     (float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y + 1,
															(float)pos.x, (float)pos.y + 1,		(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y,
							});
						indecies.insert(indecies.end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockNZ == BlockType::None) {
						texCoord = sideTexCoord;
						vertecies.insert(vertecies.end(), { (float)pos.x,		(float)pos.y,     (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y + 1,
															(float)pos.x + 1,	(float)pos.y + 1, (float)pos.z,		(float)texCoord.x,		(float)texCoord.y,
															(float)pos.x + 1,	(float)pos.y,     (float)pos.z,		(float)texCoord.x,		(float)texCoord.y + 1,
															(float)pos.x,		(float)pos.y + 1, (float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y,
							});
						indecies.insert(indecies.end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
				}
				pos.x += 1;
			}
			pos.y += 1;
			pos.x = 0;
		}
		pos.z += 1;
		pos.y = 0;
		pos.x = 0;
	}


	VertexArray* t_ChunkVA;
//	VertexBuffer* t_ChunkVB;
//	IndexBuffer* t_ChunkIB;
//
//	t_ChunkVA = VertexArray::Create();
//
//
//	t_ChunkVB = VertexBuffer::Create(vertecies.data(), vertecies.size() * sizeof(float));
//	//m_ChunkVB.reset(VertexBuffer::Create(testVertecies.data(), testVertecies.size() * sizeof(float)));
//	t_ChunkVB->SetLayout(BufferLayout{
//		{ ShaderDataType::Float3, "a_Position" },
//		{ ShaderDataType::Float2, "a_TexCoord" },
//		});
//	t_ChunkVA->AddVertexBuffer(std::make_shared<VertexBuffer>(t_ChunkVB));
//	t_ChunkIB = IndexBuffer::Create(indecies.data(), indecies.size());
//	//m_ChunkIB.reset(IndexBuffer::Create(squareIndicies, sizeof(squareIndicies)));
//	t_ChunkVA->SetIndexBuffer(std::make_shared<IndexBuffer>(t_ChunkIB));

	return t_ChunkVA;
}

void Chunk::sendIsleToRenderer()
{
	

	m_ChunkVA.reset(VertexArray::Create());
	m_ChunkVB.reset(VertexBuffer::Create(m_vertecies->data(), m_vertecies->size() * sizeof(float)));
	m_ChunkIB;

	//m_ChunkVB.reset(VertexBuffer::Create(testVertecies.data(), testVertecies.size() * sizeof(float)));
	m_ChunkVB->SetLayout(*m_layout);
	m_ChunkVA->AddVertexBuffer(m_ChunkVB);
	m_ChunkIB.reset(IndexBuffer::Create(m_indecies->data(), m_indecies->size()));
	//m_ChunkIB.reset(IndexBuffer::Create(squareIndicies, sizeof(squareIndicies)));
	m_ChunkVA->SetIndexBuffer(m_ChunkIB);

	


}

