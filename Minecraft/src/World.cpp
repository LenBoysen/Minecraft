#include "World.h"

void World::GenerateWorld()
{
	for (int x = -4; x < 4; x++) {
		for (int y = -4; y < 4; y++) {
			GenerateChunk({ x, y });
		}
	}
	
}

void World::GenerateChunk(glm::ivec2 chunckPos)
{
	if(!Exists(chunckPos))
		m_Chunks[chunckPos] = Chunk::Generate(chunckPos);
}


void World::Render()
{
	for (auto& [key, chunk] : m_Chunks) {
		chunk->Render();
	}
}

bool World::Exists(glm::ivec2 chunckPos)
{
	glm::ivec2 pos = { 0,0 };
	const glm::ivec2& pos2 = pos;

	return m_Chunks.find(chunckPos) != m_Chunks.end();
}

