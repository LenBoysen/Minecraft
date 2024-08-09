#include "Entity.h"

Object::Object(float x, float y, float z) : m_Position( x, y, z ) {

}


Object::Object() : Object(0, 0, 0) {

}


void Object::AddVertex(const glm::vec3& vert) {

	Object::m_VertexList.push_back(vert);
}

void Object::AddVertex(glm::vec3&& vert) {

	Object::m_VertexList.push_back(vert);
}

Object& Object::operator<<(const glm::vec3& vert) {

	this->AddVertex(vert);
	return *this;
}

Object&& Object::operator<<(glm::vec3&& vert) {
	this->AddVertex(vert);
	return std::move(*this);
}