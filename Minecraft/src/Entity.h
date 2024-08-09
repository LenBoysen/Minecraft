#pragma once
#include<string>
#include<vector>

#include <glm/glm.hpp>


typedef uint32_t EntityId;


class Entity {

public:
	Entity() = default;

	inline const std::string GetName() const { return m_Name; }
	inline const EntityId GetEntityId() const { return m_EntityId; }

private:
	std::string m_Name;
	EntityId m_EntityId;
	std::vector<float> m_Position;

};

class Object : public Entity {
public:
	Object(float x, float y, float z);
	Object();

	void AddVertex(const glm::vec3& vert);
	void AddVertex(glm::vec3&& vert);

	Object& operator<<(const glm::vec3& vert);

	Object&& operator<<(glm::vec3&& vert);

private:
	glm::vec3 m_Position;
	std::vector<glm::vec3> m_VertexList;

};


class Camera : public Entity {
public:
	Camera() = default;
private:

};