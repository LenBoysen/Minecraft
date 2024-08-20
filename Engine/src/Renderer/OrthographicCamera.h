#pragma once
#include "glm/glm.hpp"


enum class CameraType {
	None = 0, Orthographic
};


class OrthographicCamera{
public:
	OrthographicCamera(float left, float right, float bottom, float top);


	inline void SetPosition(const glm::vec3& position) { m_Position = position; CalculateViewMatrix(); }
	inline const glm::vec3 GetPosition() const { return m_Position; }

	inline void SetRotation(float rotation) { m_Rotation = rotation; CalculateViewMatrix(); }
	inline const float GetRotation() const { return m_Rotation; }


	inline const glm::mat4x4& GetViewMatrix() const { return m_ViewMatrix; }
	inline const glm::mat4x4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	inline const glm::mat4x4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
private:

	void CalculateViewMatrix();
	//void CalculateProjectionMatrix();
	//void CalculateVPMatrix();

private:
	glm::mat4x4 m_ProjectionMatrix;
	glm::mat4x4 m_ViewMatrix;
	glm::mat4x4 m_ViewProjectionMatrix;

	glm::vec3   m_Position = {0.0f, 0.0f, 0.0f};
	float m_Rotation = 0.0f;
};

