#include "Player.h"
#include "glm/ext/matrix_transform.hpp"
#include "World.h"
#include <xutility>
#include "Core/TimeStep.h"
#include <iostream>
#include "glm/fwd.hpp"
#include <cassert>
//#include "Core/TimeStep.h"



std::ostream& operator<<(std::ostream& ostream, const glm::vec3& vec3) {
	return ostream << "X: " << vec3.x << " Y: " << vec3.y << " Z: " << vec3.z;
}

float Player::DistanceToGround()
{
	glm::ivec3 playerPosition = glm::floor(m_Position);

	glm::ivec3 chunkBlockPos(playerPosition.x % 16, playerPosition.y, playerPosition.z % 16);
	chunkBlockPos = glm::ivec3{ (chunkBlockPos.x + 16) % 16, chunkBlockPos.y, (chunkBlockPos.z + 16) % 16 };

	glm::ivec2 playerChunk((playerPosition.x - chunkBlockPos.x) / 16, (playerPosition.z - chunkBlockPos.z) / 16);

	//m_OnGround =
		

	return 0.0f;

}

Player* Player::s_instance = new Player();

Player::Player()
	: m_Position({ 0.0f, 500.0f, 0.0f }), m_Rotation({ -90.0f, 0.0f, 0.0f })
{
	

	
	m_Camera.reset(new PerspectiveCamera(45.0f, 1.6f + 1.6f, 0.9f + 0.9f, 0.01f, 850.0f));
	
	SetPosition(m_Position);
	SetRotation(m_Rotation);
	m_CameraOperator.reset(new CameraOperator(m_Camera));


}


void Player::Spawn(Ref<World> world, glm::vec2)
{
	
	if (world->Exists({ 0,0 }))
		SetPosition({ 0, (- world->DistanceToGround({0,0,0})), 0});

	
	m_OnGround = true;
}

float Player::GetDistanceToGround(Ref<World> world) {
	return GetDistanceToGround(world, m_Position);
}

float Player::GetDistanceToGround(Ref<World> world, glm::vec3 position, int& blockingBlocks) {

	int blocks = 0;

	glm::vec3 insideBlockPosition = glm::mod(position, 1.0f);

	glm::ivec2 blockRegion = { 0, 0 };

	if (insideBlockPosition.x > 0.7f)
		blockRegion.x = 1;
	else if (insideBlockPosition.x < 0.3f)
		blockRegion.x = -1;

	if (insideBlockPosition.z > 0.7f)
		blockRegion.y = 1;
	else if (insideBlockPosition.z < 0.3f)
		blockRegion.y = -1;

	std::vector<float> distanceToGround;

	distanceToGround.push_back(world->DistanceToGround(position));

	if (blockRegion.x != 0) {
		distanceToGround.push_back(world->DistanceToGround(position + glm::vec3({ 0.3f * glm::sign(blockRegion.x), 0, 0 })));
		if (distanceToGround.back() < 0.0f) {
			if (glm::sign(blockRegion.x) > 0)
				blocks |= (1 << 0);
			else
				blocks |= (1 << 1);
		}
	}
	if (blockRegion.y != 0) {
		distanceToGround.push_back(world->DistanceToGround(position + glm::vec3({ 0, 0, 0.3f * glm::sign(blockRegion.y) })));
		if (distanceToGround.back() < 0.0f) {
			if (glm::sign(blockRegion.y) > 0)
				blocks |= (1 << 2);
			else
				blocks |= (1 << 3);
		}
	}
	if (blockRegion.x != 0 && blockRegion.y != 0) {
		distanceToGround.push_back(world->DistanceToGround(position + glm::vec3({ 0.3f * glm::sign(blockRegion.x), 0, 0.3f * glm::sign(blockRegion.y) })));
		if (distanceToGround.back() < 0.0f)
			blocks |= (1 << 4);
	}
	blockingBlocks = blocks;
	return std::min_element(distanceToGround.begin(), distanceToGround.end())[0];
}

float Player::GetDistanceToGround(Ref<World> world, glm::vec3 position) {

	glm::vec3 insideBlockPosition = glm::mod(position, 1.0f);

	glm::ivec2 blockRegion = { 0, 0 };

	if (insideBlockPosition.x > 0.7f)
		blockRegion.x = 1;
	else if (insideBlockPosition.x < 0.3f)
		blockRegion.x = -1;

	if (insideBlockPosition.z > 0.7f)
		blockRegion.y = 1;
	else if (insideBlockPosition.z < 0.3f)
		blockRegion.y = -1;

	std::vector<float> distanceToGround;

	distanceToGround.push_back(world->DistanceToGround(position));

	if (blockRegion.x != 0) {
		distanceToGround.push_back(world->DistanceToGround(position + glm::vec3({0.3f * glm::sign(blockRegion.x), 0, 0})));
	}
	if (blockRegion.y != 0) {
		distanceToGround.push_back(world->DistanceToGround(position + glm::vec3({ 0, 0, 0.3f * glm::sign(blockRegion.y) })));
	}
	if (blockRegion.x != 0 && blockRegion.y != 0) {
		distanceToGround.push_back(world->DistanceToGround(position + glm::vec3({ 0.3f * glm::sign(blockRegion.x), 0, 0.3f * glm::sign(blockRegion.y) })));
	}

	return std::min_element(distanceToGround.begin(), distanceToGround.end())[0];
}

void Player::MovePlayer(glm::vec3 moveVector, float timeStep, Ref<World> world) {


	if (!world->IsChunkLoad(m_Position)) {
		world->ReloadChunk(m_Position);
		return;
	}
	glm::vec3 worldPosition = GetPosition();
	glm::vec3 rotation = GetRotation();
	glm::mat4 rotationMaterix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f });
	glm::vec3 moveVectorObjectSpace = rotationMaterix * glm::vec4(moveVector, 1.0f);


	//const Minecraft* minecraft = Minecraft::Get();

	float distanceToGround = GetDistanceToGround(world);


	if (m_OnGround) {
		m_fallingSpeed = 0;
		m_Momentum.y = 0;
		if (m_Jumping) {
			m_Jumping = false;
			std::cout << "on Ground " << std::endl;
		}
		if (distanceToGround > 0.01f) {
			m_OnGround = false;
		}
		if (timeStep > 0.5)
			return;
		m_Momentum -= timeStep * m_DeAccelartion  * m_Momentum;
		m_Momentum += moveVectorObjectSpace * m_Accelaration;
	}
	else {
		m_Momentum -= timeStep * m_DeAccelartion * 0.01f * m_Momentum;
		m_Momentum += moveVectorObjectSpace * m_Accelaration * 0.01f;
	}
	if (m_OnGround) {
		if (m_Sprinting) {
			if (glm::length(m_Momentum * glm::vec3(1, 0, 1)) > m_MaxSprintSpeed)
				m_Momentum = glm::normalize(m_Momentum) * m_MaxSprintSpeed;
		}
		else {
			if (glm::length(m_Momentum * glm::vec3(1, 0, 1)) > m_MaxMovementSpeed)
				m_Momentum = glm::normalize(m_Momentum) * m_MaxMovementSpeed;
		}
	}


	switch (m_GameMode)
	{
	case Player::GameMode::Survival:
		if (!m_OnGround) {
			m_fallingSpeed += m_Gravity * timeStep;
			if (m_fallingSpeed < (-m_MaxFallingSpeed))
				m_fallingSpeed = -m_MaxFallingSpeed;

		}

		break;
	case Player::GameMode::Creative:
		if (!m_Hover && !m_OnGround) {
			m_Momentum += m_Gravity;
		}
		break;
	}

	

	glm::vec3 nextPosition = m_Position + m_Momentum * timeStep;

	
	
	glm::vec3 checkPosition = nextPosition + glm::sign(m_Momentum.x) * glm::vec3(0.3f, 0, 0);

	glm::vec3 finalPosition = nextPosition;

	int clippingBlocks;

	if (GetDistanceToGround(world, nextPosition, clippingBlocks) < 0.0f) {
		if (clippingBlocks & (1 << 0)) {
			finalPosition.x = ceil(finalPosition.x) - 0.300001f;
			m_Momentum.x = 0;
		}
		if (clippingBlocks & (1 << 1)) {
			finalPosition.x = floor(finalPosition.x) + 0.300001f;
			m_Momentum.x = 0;
		}

		if (clippingBlocks & (1 << 2)) {
			finalPosition.z = ceil(finalPosition.z) - 0.300001f;
			m_Momentum.z = 0;
		}
		if (clippingBlocks & (1 << 3)) {
			finalPosition.z = floor(finalPosition.z) + 0.300001f;
			m_Momentum.z = 0;
		}


		if (clippingBlocks & (1 << 4)) {
			glm::vec3 blockPosition = m_Position - glm::floor(nextPosition);
			glm::vec3 nextblockPosition = nextPosition - glm::floor(nextPosition);
			
			
			if (nextblockPosition.x > 0.5) {
				if (nextblockPosition.z > 0.5) {
					if (blockPosition.x > 0.7f) {
						finalPosition.z = floor(nextPosition.z) + 0.699999f;
						m_Momentum.z = 0;
					}
					else if (blockPosition.z > 0.7f) {
						finalPosition.x = floor(nextPosition.x) + 0.699999f;
						m_Momentum.x = 0;
					}
					else {
						assert(true, "Continue and see if this needs fixing");
					}
				}
				else {
					if (blockPosition.x > 0.7f) {
						finalPosition.z = floor(nextPosition.z) + 0.300001f;
						m_Momentum.z = 0;
					}
					else if (blockPosition.z < 0.3f) {
						finalPosition.x = floor(nextPosition.x) + 0.699999f;
						m_Momentum.x = 0;
					}
					else {
						assert(true, "Continue and see if this needs fixing");
					}
				}
			}
			else {
				if (nextblockPosition.z > 0.5) {
					if (blockPosition.x < 0.3f) {
						finalPosition.z = floor(nextPosition.z) + 0.699999f;
						m_Momentum.z = 0;
					}
					else if (blockPosition.z > 0.7f) {
						finalPosition.x = floor(nextPosition.x) + 0.300001f;
						m_Momentum.x = 0;
					}
					else {
						assert(true, "Continue and see if this needs fixing");
					}
				}
				else {
					if (blockPosition.x < 0.3f) {
						finalPosition.z = floor(nextPosition.z) + 0.300001f;
						m_Momentum.z = 0;
					}
					else if (blockPosition.z < 0.3f) {
						finalPosition.x = floor(nextPosition.x) + 0.300001f;
						m_Momentum.x = 0;
					}
					else {
						assert(true, "Continue and see if this needs fixing");
					}
				}
			}
			//if (blockPosition.x > 0.7f || blockPosition.x < nextblockPosition.x)
			//	if(nextblockPosition.z > 0.5)
			//		finalPosition.z = floor(nextPosition.z) + 0.699999f;
			//	else
			//		finalPosition.z = floor(nextPosition.z) + 0.311111f;
			//
			//if (blockPosition.z > 0.7f)
			//	if (nextblockPosition.x > 0.5)
			//		finalPosition.x = floor(nextPosition.x) + 0.699999f;
			//	else
			//		finalPosition.x = floor(nextPosition.x) + 0.311111f;
			//
			//
			//
			//if (blockPosition.x < 0.3f)
			//	if (nextblockPosition.z > 0.5)
			//		finalPosition.z = floor(nextPosition.z) + 0.699999f;
			//	else
			//		finalPosition.z = floor(nextPosition.z) + 0.311111f;
			//
			//if (blockPosition.z < 0.3f)
			//	if (nextblockPosition.x > 0.5)
			//		finalPosition.x = floor(nextPosition.x) + 0.699999f;
			//	else
			//		finalPosition.x = floor(nextPosition.x) + 0.311111f;



			//finalPosition.x = round(finalPosition.x);
		}

	}

	//if (world->DistanceToGround(checkPosition) < 0.0f) {
	//	if (glm::sign(m_Momentum.x) > 0)
	//		finalPosition.x = floor(checkPosition.x) - 0.3;
	//	else
	//		finalPosition.x = ceil(checkPosition.x) + 0.3;
	//	m_Momentum.x = 0;
	//}
	//
	//checkPosition = nextPosition + glm::sign(m_Momentum.z) * glm::vec3(0, 0, 0.3);
	//if (world->DistanceToGround(checkPosition) < 0.0f) {
	//	if (glm::sign(m_Momentum.z) > 0)
	//		finalPosition.z = floor(checkPosition.z) - 0.3;
	//	else
	//		finalPosition.z = ceil(checkPosition.z) + 0.3;
	//	m_Momentum.z = 0;
	//}

	


	distanceToGround = GetDistanceToGround(world, finalPosition);

	if (distanceToGround + m_fallingSpeed * timeStep < 0 ) {
		m_Momentum.y = (-distanceToGround / timeStep);
		m_OnGround = true;
		std::cout << m_Position << " " << finalPosition << " dist to ground final: " << distanceToGround << std::endl;
	}
	else {
		m_Momentum.y = m_fallingSpeed;
		
	}
	finalPosition.y = finalPosition.y + m_Momentum.y * timeStep;

	SetPosition(finalPosition);
}


void Player::RotatePlayer(glm::vec3 rotationVector) {


	SetRotation(m_Camera->GetRotation() + rotationVector);
}

void Player::Jump()
{
	if (m_OnGround && !m_Jumping) {
		float timer = TimeStep::GetTimer("Jump");
		std::cout << timer;

		if (0.0005 < timer && timer < 0.5)
		{
			std::cout << std::endl;
			return;
		}
		TimeStep::ResetTimer("Jump");
		std::cout << " used " << std::endl;
		m_OnGround = false;
		m_Jumping = true;
		m_fallingSpeed = m_JumpingAccelartion;
	}
}

