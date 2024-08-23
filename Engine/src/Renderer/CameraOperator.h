#pragma once
#include "Core.h"
#include "PerspectiveCamera.h"

class CameraOperator {
public:
	inline CameraOperator(Ref<PerspectiveCamera> camera)
		: m_Camera(camera) { }

	void SetPosition(glm::vec3 position) { m_Camera->SetPosition(position); }
	inline void SetFov(float fov) { m_Camera->SetFov(fov); }

	inline const Ref<PerspectiveCamera> GetCamera() const { return m_Camera; }

	void MoveCamera(glm::vec3 moveVector);

private:
	Ref<PerspectiveCamera> m_Camera;
};