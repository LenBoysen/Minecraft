#include "CameraOperator.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


void CameraOperator::MoveCamera(glm::vec3 moveVector){
	glm::vec3 worldPosition = m_Camera->GetPosition();
	glm::vec3 rotation = m_Camera->GetRotation();
	glm::mat4 rotationMaterix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), {0.0f, 1.0f, 0.0f});
	glm::vec3 moveVectorObjectSpace = rotationMaterix * glm::vec4(moveVector, 1.0f);

	glm::mat4 translationMatrixObjectSpace = glm::translate(glm::mat4(1.0f), moveVectorObjectSpace);
	glm::vec4 finalPosition4f = translationMatrixObjectSpace * glm::vec4(worldPosition, 1.0f);
	m_Camera->SetPosition({ finalPosition4f.x, finalPosition4f.y, finalPosition4f.z});
}
