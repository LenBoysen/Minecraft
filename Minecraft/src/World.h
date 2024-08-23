#pragma once

#include "Chunk.h"
#include <unordered_map>
#include <memory>






class World {
public:
	void GenerateWorld();
	void GenerateChunk(glm::ivec2);
	void Render();

private:
	bool Exists(glm::ivec2 chunckPos);

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