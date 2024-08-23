#pragma once
#include "glm/glm.hpp"
#include "Renderer/VertexArray.h"
#include <memory>
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

enum class BlockType : uint16_t {
	None		= 0,
	Stone		= 0b1000000000000000,
	Dirt		= 0b0100000000000000,
	Grass		= 0b1100000000000000,
	Wood		= 0b0010000000000000,
	Plank		= 0b1010000000000000,
	Obsidian	= 0b0110000000000000,
	Bedrock		= 0b1110000000000000,

};


class Block {
	
public:
	inline static std::shared_ptr<VertexArray> GetVertexArray() { return s_Block->m_CubeVA; }
	inline static std::shared_ptr<Shader> GetShader() { return s_Block->m_CubeTextureShader; }
	inline static std::shared_ptr<Texture2D> GetTexture() { return s_Block->m_Texture; }
	static const void GenerateVertexArray();

private:
	static Block* s_Block;
	std::shared_ptr<VertexArray> m_CubeVA;
	std::shared_ptr<VertexBuffer> m_CubeVB;
	std::shared_ptr<IndexBuffer> m_CubeIB;
	std::shared_ptr<Texture2D> m_Texture;
	std::shared_ptr<Shader> m_CubeTextureShader;
};



