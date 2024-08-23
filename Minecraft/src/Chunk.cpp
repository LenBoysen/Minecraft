#include "Chunk.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Application.h"
#include <memory>
#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/ext/matrix_transform.hpp"

Chunk::Chunk(glm::ivec2 chunkPosition)
	: m_ChunkPosition(chunkPosition)
{
	for (int z = 0; z < 512; z++) {
		for (int y = 0; y < 16; y++) {
			for (int x = 0; x < 16; x++) {
				switch (z) {
				case 0:
					blocks[x + 16 * y + 256 * z] = BlockType::Bedrock;
					break;
				case 1: case 2:
					blocks[x + 16 * y + 256 * z] = BlockType::Dirt;
					break;
				case 3:
					blocks[x + 16 * y + 256 * z] = BlockType::Grass;
					break;
				}
			}
		}
	}
	Block::GenerateVertexArray();
}

std::shared_ptr<Chunk> Chunk::Generate(glm::ivec2 chunkPosition)
{
	return std::make_shared<Chunk>(chunkPosition);
}

void Chunk::Render()
{
	auto m_textureShader = Application::Get().getShaderLibrary().Get("Texture");
	m_textureShader->Bind();
	for (int z = 0; z < 4; z++) {
		for (int y = 0; y < 16; y++) {
			for (int x = 0; x < 16; x++) {
				glm::vec2 sideTexCoord;
				glm::vec2 topTexCoord;
				glm::vec2 bottomTexCoord;
				glm::vec2 multColorCoord;
				int texMode;
				switch (blocks[x + 16 * y + 256 * z]) {
				case BlockType::Stone:		texMode = 0; sideTexCoord = { 1, 0 };  topTexCoord = { 0, 0 };  bottomTexCoord = { 0, 0 }; multColorCoord = {0, 0};  break;
				case BlockType::Dirt: 		texMode = 0; sideTexCoord = { 2, 0 };  topTexCoord = { 0, 0 };  bottomTexCoord = { 0, 0 }; multColorCoord = {0, 0};  break;
				case BlockType::Grass:		texMode = 1; sideTexCoord = { 3, 0 };  topTexCoord = { 0, 0 };  bottomTexCoord = { 0, 0 }; multColorCoord = { (16*3 + 2)/ 256.0f, (-1 + 16*16 - 2)/ 256.0f };  break;
				case BlockType::Plank:		texMode = 0; sideTexCoord = { 4, 0 };  topTexCoord = { 0, 0 };  bottomTexCoord = { 0, 0 }; multColorCoord = {0, 0};  break;
				case BlockType::Wood: 		texMode = 0; sideTexCoord = { 4, 1 };  topTexCoord = { 0, 0 };  bottomTexCoord = { 0, 0 }; multColorCoord = {0, 0};  break;
				case BlockType::Obsidian: 	texMode = 0; sideTexCoord = { 5, 2 };  topTexCoord = { 0, 0 };  bottomTexCoord = { 0, 0 }; multColorCoord = {0, 0};  break;
				case BlockType::Bedrock: 	texMode = 0; sideTexCoord = { 1, 1 };  topTexCoord = { 0, 0 };  bottomTexCoord = { 0, 0 }; multColorCoord = {0, 0};  break;
				default:					texMode = 0; sideTexCoord = { 15, 1 }; topTexCoord = { 0, 0 };  bottomTexCoord = { 0, 0 }; multColorCoord = {0, 0}; break;
				}
				std::dynamic_pointer_cast<OpenGLShader>(m_textureShader)->UploadUniformInt("u_TexMode", texMode);
				std::dynamic_pointer_cast<OpenGLShader>(m_textureShader)->UploadUniformFloat2("u_SideTexture", sideTexCoord);
				std::dynamic_pointer_cast<OpenGLShader>(m_textureShader)->UploadUniformFloat2("u_TopTexture", topTexCoord);
				//std::dynamic_pointer_cast<OpenGLShader>(m_textureShader)->UploadUniformFloat2("u_BotTexture", bottomTexCoord);
				//std::dynamic_pointer_cast<OpenGLShader>(m_textureShader)->UploadUniformFloat2("u_MultColorCoord", multColorCoord);
				
				glm::mat4 transfrom = glm::translate(glm::mat4(1.0f), { x * 1.0f + m_ChunkPosition.x * 16.0f, (z + (16*4 - glm::sin((x + m_ChunkPosition.x * 16)/20.0f) * int(glm::sqrt( (x + m_ChunkPosition.x * 16)* (x + m_ChunkPosition.x * 16) + (y + m_ChunkPosition.y * 16)*( y + m_ChunkPosition.y * 16) )) ) / 2.0f)  * 1.0f , y * 1.0f + m_ChunkPosition.y * 16.0f});
				Renderer::Submit(m_textureShader, Block::GetVertexArray(), transfrom);
			}
		}
	}
	

	//s_Block->m_Texture = Texture2D::Create("assets/textures/terrain.png");
	//s_Block->m_CubeTextureShader->Bind();
	//s_Block->m_CubeTextureShader->UploadUniformInt("u_Texture", 0);
	//std::dynamic_pointer_cast<OpenGLShader>(s_Block->m_CubeTextureShader)->Bind();
	//std::dynamic_pointer_cast<OpenGLShader>(s_Block->m_CubeTextureShader)->UploadUniformInt("u_Texture", 0);

	
}

void Chunk::GenerateIsle()
{
	for (int z = 0; z < 512; z++) {
		for (int y = 0; y < 16; y++) {
			for (int x = 0; x < 16; x++) {
				//blocks[x + 16 * y + 256 * z]
			}
		}
	}
}
