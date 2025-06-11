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
#include "FastNoise/FastNoise.h"
#include "FastSIMD/FastSIMD.h"
#include <mutex>


Chunk::Chunk(glm::ivec2 chunkPosition)
	: m_ChunkPosition(chunkPosition)
{

	//Block::GenerateVertexArray();	s_Block->m_CubeIB = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(squareIndicies, 36));

	

	ShaderLibrary& sl = Application::Get().getShaderLibrary();

	m_ChunkTextureShader = sl.Load("Minecraft/assets/shaders/Texture.glsl");



	m_Texture = Texture2D::Create("Minecraft/assets/textures/terrain.png");

	m_Texture->Bind(0);
	//s_Block->m_CubeTextureShader->UploadUniformInt("u_Texture", 0);
	std::dynamic_pointer_cast<OpenGLShader>(m_ChunkTextureShader)->Bind();
	std::dynamic_pointer_cast<OpenGLShader>(m_ChunkTextureShader)->UploadUniformInt("u_Texture", 0);
	std::dynamic_pointer_cast<OpenGLShader>(m_ChunkTextureShader)->UploadUniformFloat4("u_GrasMultColor", glm::vec4({ 0.35f, 0.78f, 0.29f, 1 }));
	//GenerateIsles();
	
	

	//SetFutureRet(&ret);
	//ret.wait();


	//sendIsleToRenderer(ret.get());

//	std::thread t1(&Chunk::GenerateIsles, this);
//	t1.join();

}

std::shared_ptr<Chunk> Chunk::AsyncGenerate() {
	return AsyncGenerate(m_ChunkPosition);
}
std::shared_ptr<Chunk> Chunk::AsyncGenerate(glm::ivec2 chunkPosition){
	Chunk* chunk = new Chunk(chunkPosition);
	chunk->m_IsCalculatingRenderData = true;
	chunk->asyncChunkGenerationTask = std::async(std::launch::async, &Chunk::AsyncGenereateChunk, chunk);
	return std::shared_ptr<Chunk>(chunk);
}



void Chunk::Render()
{
	//std::cout << TimeStep::GetTime() << " - Render Chunk: x:" << m_ChunkPosition.x << " y:" << m_ChunkPosition.y << std::endl;
	if (!m_ReadyToRender) {
		if (m_BuffersExists) {
			assert(m_vertecies);
			assert(m_indecies);
			sendIsleToRenderer();
			m_IsCalculatingRenderData = false;
		}
		else {
			if (!m_FallbackVA)
				return;
			
		}
	}
	std::dynamic_pointer_cast<OpenGLShader>(m_ChunkTextureShader)->UploadUniformInt("u_Texture", 0);
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
		ResetBuffer();
		assert(asyncTask.valid());
		asyncTask = std::async(std::launch::async, &Chunk::GenereateRenderData, this);
		
	}
	else {
		if (m_WorldGenDone) {
			ResetBuffer();
			m_WorldGenDone = false;
			m_ReadyToRender = false;
			assert(asyncTask.valid());
			asyncTask = std::async(std::launch::async, &Chunk::GenereateRenderData, this);
		}
		else {
			m_WorldGenHotReload = true;
		}
		
	}

}

bool Chunk::IsLoaded() const
{
	return m_BuffersExists;
	//return m_ReadyToRender;
}

bool Chunk::IsCalculatingRenderData() const
{
	return m_IsCalculatingRenderData;
}

void Chunk::GenereateChunk() {
	std::cout << TimeStep::GetTime() << " - Generate Chunk: x:" << m_ChunkPosition.x << " z: " << m_ChunkPosition.y << std::endl;
	FastNoise::SmartNode<> fastnoise;
	fastnoise = FastNoise::New<FastNoise::Perlin>(FastSIMD::eLevel::Level_AVX512);

	std::vector<float> noiseMap(16 * 16);
	//noiseMap.reserve(16 * 16);
	fastnoise->GenUniformGrid2D(noiseMap.data(), m_ChunkPosition.x * 16, m_ChunkPosition.y * 16, 16, 16, 0.0052587890625f, 0);
	for (int x = 0; x < 16; x++) {
		for (int z = 0, height = 0; z < 16; z++) {
			height = 256 + noiseMap[x + 16 * z] * 32;
			for (int y = 0; y < 512; y++) {
				if (y == 0)							blocks[x + 256 * y + 16 * z] = BlockType::Bedrock;
				else if (y > 0 && y < height)		blocks[x + 256 * y + 16 * z] = BlockType::Dirt;
				else if (y == height)				blocks[x + 256 * y + 16 * z] = BlockType::Grass;
				else								blocks[x + 256 * y + 16 * z] = BlockType::None;

			}
		}
	}
	m_WorldGenDone = true;
	m_FallbackExists = true;
}

void Chunk::SyncGenereateChunk() {
	m_WorldGenDone = false;
	m_IsCalculatingRenderData = true;
	m_ReadyToRender = false;
	m_WorldGenHotReload = false;
	GenereateChunk();
	GenereateRenderData();
}

void Chunk::AsyncGenereateChunk(){
	m_WorldGenDone = false;
	m_IsCalculatingRenderData = true;
	m_ReadyToRender = false;
	m_WorldGenHotReload = false;
	GenereateChunk();
	asyncTask = std::async(std::launch::async, &Chunk::GenereateRenderData, this);
}

void Chunk::AsyncReloadChunk() {

	if (m_IsCalculatingRenderData)
		return;

	std::cout << TimeStep::GetTime() << " - Reload Chunk: x:" << m_ChunkPosition.x << " z: " << m_ChunkPosition.y << std::endl;
	m_IsCalculatingRenderData = true;
	if (m_FallbackExists)
		asyncTask = std::async(std::launch::async, &Chunk::GenereateRenderData, this);
	else
		asyncChunkGenerationTask = std::async(std::launch::async, &Chunk::AsyncGenereateChunk, this);
}

void Chunk::ResetBuffer() {
	m_BuffersExists = false;
	m_vertecies.reset(new std::vector<float>);
	m_indecies.reset(new std::vector<uint32_t>);
}

void Chunk::GenereateRenderData() {
	m_IsCalculatingRenderData = true;

	unsigned int index = 0;
	glm::vec2 texCoord;
	ResetBuffer();
	m_vertecies->reserve(16 * 16 * 512);
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
						m_vertecies->insert(m_vertecies->end(), {	(float)pos.x + 1,	(float)pos.y,		(float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y + 1,	(float)1, (float)0, (float)0,
																	(float)pos.x + 1,	(float)pos.y + 1,	(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y,		(float)1, (float)0, (float)0,
																	(float)pos.x + 1,	(float)pos.y,		(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y + 1,	(float)1, (float)0, (float)0,
																	(float)pos.x + 1,	(float)pos.y + 1,	(float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y,		(float)1, (float)0, (float)0,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockNX == BlockType::None) {
						texCoord = sideTexCoord;
						m_vertecies->insert(m_vertecies->end(), {	(float)pos.x,		(float)pos.y,		(float)pos.z,		(float)texCoord.x,		(float)texCoord.y + 1,	(float)-1,(float)0, (float)0,
																	(float)pos.x,		(float)pos.y + 1,	(float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y,		(float)-1,(float)0, (float)0,
																	(float)pos.x,		(float)pos.y,		(float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y + 1,	(float)-1,(float)0, (float)0,
																	(float)pos.x,		(float)pos.y + 1,	(float)pos.z,		(float)texCoord.x,		(float)texCoord.y,		(float)-1,(float)0, (float)0,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockPY == BlockType::None) {
						texCoord = topTexCoord;
						m_vertecies->insert(m_vertecies->end(), { (float)pos.x,			(float)pos.y + 1,	(float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y + 1,	(float)0, (float)1, (float)0,
																	(float)pos.x + 1,	(float)pos.y + 1,	(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y,		(float)0, (float)1, (float)0,
																	(float)pos.x,		(float)pos.y + 1,	(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y + 1,	(float)0, (float)1, (float)0,
																	(float)pos.x + 1,	(float)pos.y + 1,	(float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y,		(float)0, (float)1, (float)0,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockNY == BlockType::None) {
						texCoord = bottomTexCoord;
						m_vertecies->insert(m_vertecies->end(), {	(float)pos.x,		(float)pos.y,		(float)pos.z,		(float)texCoord.x,		(float)texCoord.y + 1,	(float)0, (float)-1,(float)0,
																	(float)pos.x + 1,	(float)pos.y,		(float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y,		(float)0, (float)-1,(float)0,
																	(float)pos.x,		(float)pos.y,		(float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y + 1,	(float)0, (float)-1,(float)0,
																	(float)pos.x + 1,	(float)pos.y,		(float)pos.z,		(float)texCoord.x,		(float)texCoord.y,		(float)0, (float)-1,(float)0,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockPZ == BlockType::None) {
						texCoord = sideTexCoord;
						m_vertecies->insert(m_vertecies->end(), { (float)pos.x,		(float)pos.y,		(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y + 1,		(float)0, (float)0, (float)1,
																	(float)pos.x + 1,	(float)pos.y + 1,	(float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y,		(float)0, (float)0, (float)1,
																	(float)pos.x + 1,	(float)pos.y,		(float)pos.z + 1,	(float)texCoord.x + 1,	(float)texCoord.y + 1,	(float)0, (float)0, (float)1,
																	(float)pos.x,		(float)pos.y + 1,	(float)pos.z + 1,	(float)texCoord.x,		(float)texCoord.y,		(float)0, (float)0, (float)1,
							});
						m_indecies->insert(m_indecies->end(), { index, index + 1, index + 2, index, index + 3, index + 1 });
						index += 4;
					}
					if (blockNZ == BlockType::None) {
						texCoord = sideTexCoord;
						m_vertecies->insert(m_vertecies->end(), {	(float)pos.x,		(float)pos.y,		(float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y + 1,	(float)0, (float)0, (float)-1,
																	(float)pos.x + 1,	(float)pos.y + 1,	(float)pos.z,		(float)texCoord.x,		(float)texCoord.y,		(float)0, (float)0, (float)-1,
																	(float)pos.x + 1,	(float)pos.y,		(float)pos.z,		(float)texCoord.x,		(float)texCoord.y + 1,	(float)0, (float)0, (float)-1,
																	(float)pos.x,		(float)pos.y + 1,	(float)pos.z,		(float)texCoord.x + 1,	(float)texCoord.y,		(float)0, (float)0, (float)-1,
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
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float3, "a_Normal" },
		}));

	if (m_WorldGenHotReload) {
		m_WorldGenHotReload = false;
		GenereateRenderData();
	}
	else {
		m_IsCalculatingRenderData = false;
		m_FallbackVA = true;
		m_BuffersExists = true;
	}
}

void Chunk::UnloadRenderData()
{
	std::cout << TimeStep::GetTime() << " - Unload Chunk: x:" << m_ChunkPosition.x << " z: " << m_ChunkPosition.y << std::endl;

	asyncChunkGenerationTask.wait();
	asyncTask.wait();
	m_ReadyToRender = false;
	m_WorldGenHotReload = false; 
	m_BuffersExists = false;
	m_FallbackVA = false;
	m_vertecies.reset();
	m_indecies.reset();
	m_ChunkIB.reset();
	m_ChunkVB.reset();
	m_ChunkVA.reset();
		
	

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

void Chunk::sendIsleToRenderer(){
	
	assert(m_vertecies);
	assert(m_indecies);
	assert(m_layout->getElements().size() > 0);

	m_FallbackVA = false;
	m_ChunkVA.reset(VertexArray::Create());
	m_ChunkVB.reset(VertexBuffer::Create(m_vertecies->data(), m_vertecies->size() * sizeof(float)));
	

	//m_ChunkVB.reset(VertexBuffer::Create(testVertecies.data(), testVertecies.size() * sizeof(float)));
	m_ChunkVB->SetLayout(*m_layout);
	m_ChunkVA->AddVertexBuffer(m_ChunkVB);
	m_ChunkIB.reset(IndexBuffer::Create(m_indecies->data(), m_indecies->size()));
	//m_ChunkIB.reset(IndexBuffer::Create(squareIndicies, sizeof(squareIndicies)));
	m_ChunkVA->SetIndexBuffer(m_ChunkIB);


	m_FallbackVA = true;
	m_ReadyToRender = true;

}

float Chunk::DistanceToGround(glm::vec3 position) const{

	glm::vec3 test = glm::floor(position);

	
	glm::ivec3 blockPosition = test;
	
	if (GetBlock(blockPosition) == BlockType::None) {
		for (int i = blockPosition.y; i >= 0 ; i--) {
			blockPosition.y = i;
			if (GetBlock(blockPosition) != BlockType::None)
				return position.y - blockPosition.y - 1.0f;
		}
		return position.y;
	}
	else {
		
		for (int i = blockPosition.y + 1; i < 512; i++) {
			blockPosition.y = i;
			if (GetBlock(blockPosition) == BlockType::None)
				return position.y - blockPosition.y;
		}
		return position.y - 512;
	}
}

