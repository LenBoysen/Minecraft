#include "World.h"
#include "Core/TimeStep.h"

void World::GenerateWorld()
{
	for (int x = -16; x < 16; x++) {
		for (int z = -16; z < 16; z++) {
			GenerateChunk({ x, z });
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

void World::SetBlock(BlockType type, glm::ivec3 pos)
{
	
	glm::ivec3 chunkBlockPos(pos.x % 16, pos.y, pos.z % 16);
	chunkBlockPos = glm::ivec3{ (chunkBlockPos.x + 16) % 16, chunkBlockPos.y, (chunkBlockPos.z + 16) % 16 };
	glm::ivec2 chunkPos((pos.x - chunkBlockPos.x) / 16, (pos.z - chunkBlockPos.z) / 16);


	if (!Exists(chunkPos))
		GenerateChunk(chunkPos);
	m_Chunks[chunkPos]->SetBlock(type, chunkBlockPos);
}

std::vector<glm::ivec3> World::CastRay(const glm::vec3 cameraPosition, const glm::vec3 cameraRotation, uint32_t blockLength)
 {
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
		short lowestAchsis;
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

bool World::Exists(glm::ivec2 chunckPos) const
{
	glm::ivec2 pos = { 0,0 };
	const glm::ivec2& pos2 = pos;

	return m_Chunks.find(chunckPos) != m_Chunks.end();
}
