#pragma once


#include "glm/glm.hpp"



class PerspectiveCamera {
public:
	PerspectiveCamera(float fov, float width, float height, float near, float far);


	inline void SetPosition(const glm::vec3& position) { m_Position = position; CalculateViewMatrix(); }
	inline const glm::vec3 GetPosition() const { return m_Position; }

	inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation.x < -90.0f ? glm::vec3(-90.0f, rotation.y, rotation.z) : rotation.x > 90.0f ? glm::vec3(90.0f, rotation.y, rotation.z) : rotation; CalculateViewMatrix(); }
	inline const glm::vec3 GetRotation() const { return m_Rotation; }


	inline void SetFov(float fov) { m_Fov = fov; CalculateProjectionMatrix(); }
	inline const float GetFov() const { return m_Fov; }

	inline const glm::mat4x4& GetTranslateMatrix() const { return m_TranslateMatrix; }
	inline const glm::mat4x4& GetRotationMatrix() const { return m_RotationMatrix; }
	inline const glm::mat4x4& GetViewMatrix() const { return m_ViewMatrix; }
	inline const glm::mat4x4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	inline const glm::mat4x4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
private:

	void CalculateViewMatrix();
	void CalculateProjectionMatrix();
	//void CalculateProjectionMatrix();
	//void CalculateVPMatrix();

private:
	glm::mat4x4 m_ProjectionMatrix;
	glm::mat4x4 m_TranslateMatrix;
	glm::mat4x4 m_RotationMatrix;
	glm::mat4x4 m_ViewMatrix;
	glm::mat4x4 m_ViewProjectionMatrix;
	

	glm::vec3   m_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3   m_Rotation = { 0.0f, 0.0f, 0.0f };
	float m_Fov = 65.0f;
	float m_Width = 0.0f;
	float m_Height = 0.0f;
	float m_Near = 0.0f;
	float m_Far = 0.0f;
};

