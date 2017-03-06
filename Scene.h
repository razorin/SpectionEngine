#ifndef __SCENE_H__
#define __SCENE_H__

#include <list>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp\anim.h"


class GameObject;

class Scene
{
public:
	Scene();
	~Scene();

	void AddGameObject(GameObject* gameobject) { gameobjects.push_back(gameobject); }
	GameObject* GetGameObject(std::string name);
	void LoadLevel(const char* path, const char* file);
	void RecursiveNodeRead(GameObject* go, aiNode& aiNode, GameObject* parentGO);

	void Draw();

private:
	std::list<GameObject *> gameobjects;
	GameObject* root;
};

#endif // __SCENE_H__