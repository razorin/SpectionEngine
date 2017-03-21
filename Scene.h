#ifndef __SCENE_H__
#define __SCENE_H__

#include <list>
#include <vector>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp\anim.h"


class GameObject;
class Mesh;
class Material;

class Scene
{
public:
	Scene();
	~Scene();

	GameObject* AddGameObject(GameObject* parent = nullptr, bool editable = false);
	void DeleteGameObject(std::string name);
	GameObject* GetGameObject(std::string code);
	void LoadLevel(const char* path, const char* file);
	void RecursiveNodeRead(GameObject* go, aiNode& aiNode, GameObject* parentGO);

	void Draw();
	bool CleanUp();

	void DrawHierarchyNodes(GameObject* go);
	void DrawRecursively(GameObject* go);

private:
	int gameObjectsCounter = 0;
	std::list<GameObject *> gameObjects;
	std::vector<Mesh *> meshes;
	std::vector<Material *> materials;
	//std::list<Animation *> animations;

	int frame = 0;
	int maxFrames;

	//TODO use the texture manager instead of this
	uint* textureIds;

public:
	GameObject* root;

};

#endif // __SCENE_H__