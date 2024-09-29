#include "World.h"
#include "Core/TimeStep.h"
#include "Player.h"
#include "Application.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include <algorithm>
#include <forward_list>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Renderer/Renderer.h"
#include "gl/GL.h"

void World::GenerateWorld()
{
	GenerateChunk({ 0, 0 });
	GenerateSkyVA();
}

void World::GenerateChunk(glm::ivec2 chunckPos)
{
	if(!Exists(chunckPos))
		m_Chunks[chunckPos] = Chunk::AsyncGenerate(chunckPos);
}
void World::AsyncGenerateChunk(glm::ivec2 chunckPos)
{
	if (!Exists(chunckPos))
		m_Chunks[chunckPos] = Chunk::AsyncGenerate(chunckPos);
}

void World::ReloadWorld()
{
	glm::ivec3 playerPosition = glm::floor(Player::Get()->GetPosition());

	glm::ivec3 chunkBlockPos(playerPosition.x % 16, playerPosition.y, playerPosition.z % 16);
	chunkBlockPos = glm::ivec3{ (chunkBlockPos.x + 16) % 16, chunkBlockPos.y, (chunkBlockPos.z + 16) % 16 };

	glm::ivec2 playerChunk((playerPosition.x - chunkBlockPos.x) / 16, (playerPosition.z - chunkBlockPos.z) / 16);

	std::vector<glm::ivec2> toUnloadChunks = m_ChunksToRender;


	m_ChunksToRender.clear();
	for (int x = -m_ChunkRenderDistance + playerChunk.x; x < m_ChunkRenderDistance + playerChunk.x; x++) {
		for (int z = -m_ChunkRenderDistance + playerChunk.y; z < m_ChunkRenderDistance + playerChunk.y; z++) {
			if (glm::length(glm::vec2(playerPosition.x / 16.0f, playerPosition.z / 16.0f) - glm::vec2(glm::ivec2(x + 0.5f, z + 0.5f))) <= m_ChunkRenderDistance) {
				if (!Exists({ x, z }))
					GenerateChunk({ x, z });
				if (!ChunkReady({ x, z }))
					ReloadChunk({ x, z });
				toUnloadChunks.erase(std::remove(toUnloadChunks.begin(), toUnloadChunks.end(), glm::ivec2(x, z)), toUnloadChunks.end());
				m_ChunksToRender.push_back(glm::ivec2(x, z));
			}
		}
	}
	for (auto& chunk : toUnloadChunks)
		m_Chunks[chunk]->UnloadRenderData();
}

void World::Render()
{
	RenderSky();

	Ref<Shader> textureShader = Application::Get().getShaderLibrary().Get("Texture");
	std::dynamic_pointer_cast<OpenGLShader>(textureShader)->UploadUniformFloat3("u_SunPosVec", GetSunPosVec());
	for (auto& chunkPos : m_ChunksToRender) {
		m_Chunks[chunkPos]->Render();
	}

	
}

void World::SetBlock(BlockType type, glm::ivec3 pos)
{
	
	glm::ivec3 chunkBlockPos(pos.x % 16, pos.y, pos.z % 16);
	chunkBlockPos = glm::ivec3{ (chunkBlockPos.x + 16) % 16, chunkBlockPos.y, (chunkBlockPos.z + 16) % 16 };
	glm::ivec2 chunkPos((pos.x - chunkBlockPos.x) / 16, (pos.z - chunkBlockPos.z) / 16);


	if (!Exists(chunkPos))
		GenerateChunk(chunkPos);

	m_Chunks[chunkPos]->SetBlock(type, chunkBlockPos);
}

std::vector<glm::ivec3> World::CastRay(const glm::vec3 cameraPosition, const glm::vec3 cameraRotation, uint32_t blockLength){
	std::vector<glm::ivec3> rayBlocks;
	glm::vec3 rayPosition = cameraPosition;
	glm::vec3 direction = { -glm::sin(glm::radians(cameraRotation.y)) * glm::cos(glm::radians(cameraRotation.x)), glm::sin(glm::radians(cameraRotation.x)), -glm::cos(glm::radians(cameraRotation.y)) * glm::cos(glm::radians(cameraRotation.x)) };
	//direction = { 0, 0, -1.0f};
	glm::ivec3 rayDirectionBoundries = { direction.x >= 0.0f ,  direction.y >= 0.0f ,  direction.z >= 0.0f };
	rayBlocks.reserve(blockLength);
	rayBlocks.push_back(glm::floor(cameraPosition));


	glm::ivec3 rayInBlock = glm::floor(rayPosition);
	glm::ivec3 chunkBlockPos((rayInBlock.x + 16) % 16, rayInBlock.y, (rayInBlock.z + 16) % 16);
	glm::ivec2 chunkPos((rayInBlock.x - chunkBlockPos.x) / 16, (rayInBlock.z - chunkBlockPos.z) / 16);
	if (!Exists(chunkPos))
		GenerateChunk(chunkPos);

	while (rayBlocks.size() < blockLength) {

		


		glm::ivec3 rayBoundriesP = rayInBlock + glm::ivec3(1);

		glm::ivec3 rayBoundries = rayInBlock + rayDirectionBoundries;

		float lamda;
		float lowest;
		glm::ivec3 nextBlock;
		lamda = (rayBoundries.x - rayPosition.x) / direction.x;
		lowest = lamda;
		nextBlock = (rayDirectionBoundries * 2 - glm::ivec3({ 1, 1, 1 })) * glm::ivec3({ 1, 0, 0 });


		lamda = (rayBoundries.y - rayPosition.y) / direction.y;
		if (lowest > lamda) {
			nextBlock = (rayDirectionBoundries * 2 - glm::ivec3({ 1, 1, 1 })) * glm::ivec3({ 0, 1, 0 });
			lowest = lamda;
		}

		lamda = (rayBoundries.z - rayPosition.z) / direction.z;
		if (lowest > lamda) {
			nextBlock = (rayDirectionBoundries * 2 - glm::ivec3({ 1, 1, 1 })) * glm::ivec3({ 0, 0, 1 });
		lowest = lamda;
	}

		rayBlocks.push_back(rayInBlock + nextBlock);

		rayPosition = direction * lowest + cameraPosition;
		rayInBlock = rayInBlock + nextBlock;

		chunkBlockPos = glm::ivec3(rayInBlock.x % 16, rayInBlock.y, rayInBlock.z % 16);
		chunkBlockPos = glm::ivec3((chunkBlockPos.x + 16) % 16, chunkBlockPos.y, (chunkBlockPos.z + 16) % 16);
		chunkPos =  glm::ivec2((rayInBlock.x - chunkBlockPos.x) / 16, (rayInBlock.z - chunkBlockPos.z) / 16);
		if (!Exists(chunkPos))
			GenerateChunk(chunkPos);
		if (m_Chunks[chunkPos]->GetBlock(chunkBlockPos) != BlockType::None)
			break;



	}
	return rayBlocks;
}

float World::DistanceToGround (glm::vec3 position)
{
	glm::ivec3 blockPosition = glm::floor(position);
	glm::ivec3 chunkBlockPos(blockPosition.x % 16, blockPosition.y, blockPosition.z % 16);
	chunkBlockPos = glm::ivec3{ (chunkBlockPos.x + 16) % 16, chunkBlockPos.y, (chunkBlockPos.z + 16) % 16 };
	glm::ivec2 chunkPos((blockPosition.x - chunkBlockPos.x) / 16, (blockPosition.z - chunkBlockPos.z) / 16);

	if (!Exists(chunkPos))
		GenerateChunk(chunkPos);
	return m_Chunks[chunkPos]->DistanceToGround(position - glm::vec3(chunkPos.x * 16.0f, 0, chunkPos.y * 16.0f));

}

bool World::IsChunkLoad(glm::vec3 postion){
	glm::ivec2 chunkPos = GetChunkPosition(postion);
	if (!Exists(chunkPos))
		return false;
	return m_Chunks[chunkPos]->IsLoaded();
}

glm::ivec2 World::GetChunkPosition(glm::vec3 position) const{
	glm::ivec3 blockPosition = glm::floor(position);
	
	
	glm::ivec3 chunkBlockPos(blockPosition.x % 16, blockPosition.y, blockPosition.z % 16);
	chunkBlockPos = { (chunkBlockPos.x + 16) % 16, chunkBlockPos.y, (chunkBlockPos.z + 16) % 16 };
	
	return { (blockPosition.x - chunkBlockPos.x) / 16, (blockPosition.z - chunkBlockPos.z) / 16 };
}

bool World::ChunkReady(glm::ivec2 chunkPos)
{
	return m_Chunks[chunkPos]->IsCalculatingRenderData() || m_Chunks[chunkPos]->IsLoaded();
}


void World::ReloadChunk(glm::vec3 blockPos)
{
	ReloadChunk(GetChunkPosition(blockPos));
}
void World::ReloadChunk(glm::ivec2 chunkPos)
{
	if (m_Chunks[chunkPos]->IsCalculatingRenderData())
		return;
	m_Chunks[chunkPos]->AsyncReloadChunk();
}

int& World::GetRenderDistanceRef()
{
	return m_ChunkRenderDistance;
}

bool World::Exists(glm::ivec2 chunckPos) const
{

	return m_Chunks.find(chunckPos) != m_Chunks.end();
}

glm::vec3 World::GetSunPosVec(){
	m_WorldTime += TimeStep::ResetTimer("WorldTime");
	return { cos(glm::radians(12*m_WorldTime)), sin(glm::radians(12*m_WorldTime)), 0.0};
}


void World::GenerateSkyVA() {
	m_SkyVA.reset(VertexArray::Create());

	std::vector<float> vertecies = {
	-0.5, -0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,

	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	-0.5, 0.5, -0.5,



	0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,

	-0.5, -0.5, -0.5,
	-0.5, 0.5, -0.5,
	-0.5, 0.5, 0.5,
	-0.5, -0.5, 0.5,


	-0.5, 0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,

	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,

	};

	Ref<VertexBuffer> vb;
	vb.reset(VertexBuffer::Create(vertecies.data(), vertecies.size() * sizeof(float)));

	BufferLayout bl = {
		{ ShaderDataType::Float3, "a_Position" },
	};

	vb->SetLayout(bl);
	m_SkyVA->AddVertexBuffer(vb);

	std::vector<uint32_t> indecies = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,

		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,


		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,


	};

	Ref<IndexBuffer> ib;
	ib.reset(IndexBuffer::Create(indecies.data(), indecies.size()));

	m_SkyVA->SetIndexBuffer(ib);

	Application::Get().getShaderLibrary().Load("assets/shaders/Sky.glsl");

	Ref<IndexBuffer> ib2rwesfsefesfsef;
	//Ref<VertexBuffer> vb2;
	//
	//
	//std::vector<float> vertecies2 = {
	//-0.5, -0.5, 0.5,
	//0.5, -0.5, 0.5,
	//0.5, 0.5, 0.5,
	//-0.5, 0.5, 0.5,
	//
	//-0.5, -0.5, -0.5,
	//0.5, -0.5, -0.5,
	//0.5, 0.5, -0.5,
	//-0.5, 0.5, -0.5,
	//
	//-0.5, -0.5, -0.5,
	//-0.5, 0.5, -0.5,
	//-0.5, 0.5, 0.5,
	//-0.5, -0.5, 0.5,
	//};
	//
	//indecies.clear();
	//
	std::vector<uint32_t> indecies2 = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
	};
	//vb2.reset(VertexBuffer::Create(vertecies2.data(), vertecies2.size() * sizeof(float)));
	//vb2->SetLayout(bl);
	//
	ib2rwesfsefesfsef.reset(IndexBuffer::Create(indecies2.data(), indecies2.size()));
	//m_SunVA.reset(VertexArray::Create());
	//m_SunVA->AddVertexBuffer(vb2);
	//m_SunVA->SetIndexBuffer(ib2);
	//
	Application::Get().getShaderLibrary().Load("assets/shaders/Sun.glsl");
}

void World::RenderSky() {

	Ref<Shader> SkyShader = Application::Get().getShaderLibrary().Get("Sky");
	std::dynamic_pointer_cast<OpenGLShader>(SkyShader)->UploadUniformFloat3("u_SkyPosition", GetSunPosVec());


	Ref<Shader> SunShader = Application::Get().getShaderLibrary().Get("Sun");
	std::dynamic_pointer_cast<OpenGLShader>(SunShader)->UploadUniformFloat3("u_SunPosVec", GetSunPosVec());

	glm::mat4 transfrom = glm::translate(glm::mat4(1.0f), { 0, 0 , 0 });

	//GLint OldCullFaceMode;
	//glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	//glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	//glCullFace(GL_BACK);
	//glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	Renderer::Submit(SkyShader, m_SkyVA, glm::mat4(1.0f));

	//Renderer::Submit(SunShader, m_SunVA, transfrom);

	//glCullFace(OldCullFaceMode);
	glDepthMask(GL_TRUE);
	//glDepthFunc(OldDepthFuncMode);
}