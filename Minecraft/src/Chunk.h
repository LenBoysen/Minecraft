#pragma once
#include "glm/glm.hpp"
#include "Block.h"
#include <memory>
#include <array>

class Chunk {
public:
	Chunk(glm::ivec2 chunkPosition);

	static std::shared_ptr<Chunk> Generate(glm::ivec2 chunkPosition);
	void Render();
private:
	void GenerateIsle();
	/*
		0b 0000000000 000 000 65536 posisble comninations
		   ^--------^ ^-^ ^-^
		   |          |   |
		   |          |   > 3 bits for nothing
		   |          > 3 bits for 8 orientations
		   > 10 bits for 1024 blocktyps
	*/
	std::array<BlockType, 16 * 16 * 512>  blocks;
	glm::ivec2 m_ChunkPosition;
};