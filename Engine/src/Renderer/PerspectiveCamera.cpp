#include "PerspectiveCamera.h"

#include "glm/gtc/matrix_transform.hpp"

PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float near, float far)
	: m_Fov(fov), m_Width(height), m_Height(width), m_Near(near), m_Far(far), m_ProjectionMatrix(glm::perspectiveFov(fov, width, height, near, far)), m_ViewMatrix(1.0f)
	
{

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = glm::perspectiveFov(m_Fov, m_Width, m_Height, m_Near, m_Far);

}



void PerspectiveCamera::CalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
		glm::rotate(glm::rotate(glm::rotate(glm::mat4(1), glm::radians(m_Rotation.y), glm::vec3(0, 1, 0)), glm::radians(m_Rotation.x), glm::vec3(1, 0, 0)), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));

	m_ViewMatrix = glm::inverse(transform);

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
