#pragma once
#include "glm/glm.hpp"
#include "Block.h"
#include <memory>
#include <array>
#include <tuple>
#include <future>

union Blocks {
public:
	uint16_t test1;
	uint16_t* test2;
};

class Chunk {
public:
	Chunk(glm::ivec2 chunkPosition);

	static std::shared_ptr<Chunk> Generate(glm::ivec2 chunkPosition);
	inline BlockType GetBlock(glm::ivec3 blockPosition) {
		return blockPosition.x < 0 || blockPosition.y < 0 || blockPosition.z < 0 || blockPosition.x >= 16 || blockPosition.y >= 511 || blockPosition.z >= 16 ? BlockType::None    :     blocks[blockPosition.x + 256 * blockPosition.y + 16 * blockPosition.z];

	}
	void Render();

	void SetBlock(BlockType type, glm::ivec3 pos);
private:
	void GenereateRenderData();
	VertexArray* GenerateIsles();
	void CreateIsle();
	void sendIsleToRenderer();

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

	std::shared_ptr<VertexArray> m_ChunkVA;
	std::shared_ptr<VertexBuffer> m_ChunkVB;
	std::shared_ptr<IndexBuffer> m_ChunkIB;
	std::shared_ptr<Texture2D> m_Texture;
	std::shared_ptr<Shader> m_ChunkTextureShader; 
	Ref<std::vector<float>> m_vertecies;
	Ref<BufferLayout> m_layout;
	Ref<std::vector<uint32_t>> m_indecies;
	std::future<void> asyncTask;
	bool m_ReadyToRender = false;
	bool m_WorldGenDone = false;
	bool m_WorldGenHotReload = false;
	bool m_FallbackExists = false;


};