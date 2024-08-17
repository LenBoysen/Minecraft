#include "Scene.h"


Scene::Scene() : m_IdCounter(0) {

}

void Scene::AddEntity(Entity&& et) {
	Scene::m_EntityMap.insert(std::pair<EntityId, Entity>(Scene::m_GenerateUniqeId(), et));
}

EntityId Scene::m_GenerateUniqeId() {
	return Scene::m_IdCounter++;
}


//Scene& Scene::operator<<(Entity&& et) {
//	this->m_EntityMap.insert(std::pair<EntityId, Entity>(Scene::m_GenerateUniqeId(), et));
//	return *this;
//}

Scene&& Scene::operator<<(Entity&& et) {
	this->m_EntityMap.insert(std::pair<EntityId, Entity>(Scene::m_GenerateUniqeId(), et));
	return std::move(*this);
}




SceneMgr::SceneMgr() : m_IdCounter(0) {

}

void SceneMgr::SetActiveScene(SceneId id) {
	SceneMgr::m_ActiveScene = SceneMgr::m_SceneMap.at(id);
}

int SceneMgr::AddScene(Scene& sc) {
	SceneId id = SceneMgr::m_GenerateUniqeId();
	SceneMgr::m_SceneMap.insert(std::pair<SceneId, Scene>(id, sc));
	return id;
}

int SceneMgr::AddScene(Scene&& sc) {
	SceneId id = SceneMgr::m_GenerateUniqeId();
	SceneMgr::m_SceneMap.insert(std::pair<SceneId, Scene>(id, sc));
	return id;
}


SceneId SceneMgr::m_GenerateUniqeId() {
	return SceneMgr::m_IdCounter++;
}