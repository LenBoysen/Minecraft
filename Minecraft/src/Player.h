#pragma once
#include "glm/glm.hpp"
#include "Renderer/CameraOperator.h"

class World;




class Player {
public:
	Player();

	inline glm::vec3 GetPosition() const { return m_Position; }
	inline void SetPosition(glm::vec3 position) { m_Position = position; m_Camera->SetPosition(position + m_CameraOffset); }

	inline glm::vec3 GetRotation() const { return m_Rotation; }
	inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; m_Camera->SetRotation(rotation); }

	inline float GetMovementSpeed() const { return m_MaxMovementSpeed; }
	inline void SetMovementSpeed(float movementSpeed) { m_MaxMovementSpeed = movementSpeed; }

	inline float GetRotationSpeed() const { return m_RotationSpeed; }
	inline void SetRotationSpeed(float rotationSpeed) { m_RotationSpeed = rotationSpeed; }

	inline glm::vec3 GetMomentum() const { return m_Momentum; }
	float GetDistanceToGround(Ref<World> world);
	float GetDistanceToGround(Ref<World> world, glm::vec3 position);

	float GetDistanceToGround(Ref<World> world, glm::vec3 position, int& blockingBlocks);
	inline bool* GetOnGroundPtr() { return &m_OnGround; }
	inline bool GetOnGround() const { return m_OnGround; }
	inline void SetOnGround(bool onGround) { m_OnGround = onGround; }


	void MovePlayer(glm::vec3 moveVector, float timeStep, Ref<World> world);
	void RotatePlayer(glm::vec3 rotationVector);
	void Jump();
	void Spawn(Ref<World> world, glm::vec2);
	inline void SetSprinting(bool sprinting) {
		
		m_Sprinting = sprinting;
	}

	inline const Ref<CameraOperator> GetCameraOperator() const { return m_CameraOperator; }

	inline static Player* Get() { return s_instance; }

private:
	enum class GameMode {
		Survival = 0, Creative = 1
	};


	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_CameraOffset = { 0.0f, 1.62f, 0.0f };
	float m_MaxMovementSpeed = 4.317f;
	float m_MaxSprintSpeed = 6.1671429f;
	float m_MovementAcceleration = 75.0f;
	float m_RotationSpeed = 0.05f;
	Ref<PerspectiveCamera> m_Camera;
	Ref<CameraOperator> m_CameraOperator;
	GameMode m_GameMode = GameMode::Survival;

	bool m_OnGround = false;
	glm::vec3 m_Momentum = { 0.0f, 0.0f, 0.0f };
	float m_Gravity = -32.0f;
	float m_DeAccelartion = 25.0f;
	float m_JumpingAccelartion = 8.0f;
	float m_fallingSpeed = 0.0f;
	bool m_Hover = false;
	bool m_Jumping = false;
	bool m_Sprinting = false;
	float DistanceToGround();
	float m_MaxFallingSpeed = 256.0f;
	float m_Accelaration = 0.06f;
	//friend float World::DistanceToGround(glm::vec3);
private:
	static Player* s_instance;
};
