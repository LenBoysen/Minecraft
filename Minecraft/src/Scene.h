#pragma once
#include "Entity.h"
#include<unordered_map>








typedef uint32_t SceneId;

class Scene {
public:
	Scene();

	inline const std::unordered_map<EntityId, Entity>& GetEntityMap() const { return m_EntityMap; }
	void AddEntity(Entity&& et);
	//Scene& operator<<(Entity&& et);
	Scene&& operator<<(Entity&& et);
private:

	EntityId m_GenerateUniqeId();
	

private:
	std::unordered_map<EntityId, Entity> m_EntityMap;
	EntityId m_IdCounter;
};



class SceneMgr {
public:
	SceneMgr();
	
	inline const Scene& GetActiveScene() const { return m_ActiveScene; }

	int AddScene(Scene& sc);
	int AddScene(Scene&& sc);
	void SetActiveScene(SceneId id);
	
private:
	SceneId m_GenerateUniqeId();
	SceneId m_IdCounter;
	std::unordered_map<SceneId, Scene> m_SceneMap;
	Scene& m_ActiveScene = *new Scene();
};

