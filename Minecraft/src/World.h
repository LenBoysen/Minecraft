#pragma once

#include "Chunk.h"
#include <unordered_map>
#include <memory>






class World {
public:
	void GenerateWorld();
	void GenerateChunk(glm::ivec2);
	void AsyncGenerateChunk(glm::ivec2);
	void ReloadWorld();
	void Render();
	void SetBlock(BlockType type, glm::ivec3 pos);

	std::vector<glm::ivec3> CastRay(const glm::vec3 param1, const glm::vec3 param2, uint32_t blockLength = 16);
	float DistanceToGround(glm::vec3 position);
	bool IsChunkLoad(glm::vec3 m_Position);
	bool Exists(glm::ivec2 chunckPos) const;
	int& GetRenderDistanceRef();
	void ReloadChunk(glm::ivec2 chunkPos);
	void ReloadChunk(glm::vec3 blockPos);
private:
	void GenerateSkyVA();
	void RenderSky();
	Ref<VertexArray> m_SkyVA;
	Ref<VertexArray> m_SunVA;
	glm::ivec2 GetChunkPosition(glm::vec3 position) const;
	struct vec2_hash {
		std::size_t operator () (const glm::ivec2& p) const {
			auto h1 = std::hash<int>{}(p.x);
			auto h2 = std::hash<int>{}(p.y);
			return h1 ^ h2;
		}
	};

	int m_ChunkRenderDistance = 2;
	std::vector<glm::ivec2> m_ChunksToRender;
	bool ChunkReady(glm::ivec2 chunkPos);
private:

	std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, World::vec2_hash> m_Chunks;


	float m_WorldTime = 0;
	glm::vec3 GetSunPosVec();
};