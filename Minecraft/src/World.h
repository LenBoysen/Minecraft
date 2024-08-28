#pragma once

#include "Chunk.h"
#include <unordered_map>
#include <memory>






class World {
public:
	void GenerateWorld();
	void GenerateChunk(glm::ivec2);
	void Render();
	void SetBlock(BlockType type, glm::ivec3 pos);

	std::vector<glm::ivec3> CastRay(const glm::vec3 param1, const glm::vec3 param2, uint32_t blockLength = 16);
private:
	bool Exists(glm::ivec2 chunckPos) const;

private:
	struct vec2_hash {
		std::size_t operator () (const glm::ivec2& p) const {
			auto h1 = std::hash<int>{}(p.x);
			auto h2 = std::hash<int>{}(p.y);
			return h1 ^ h2;
		}
	};
	//std::vector<glm::ivec2> m_GeneratedChunks
	std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, World::vec2_hash> m_Chunks;

	
	
};