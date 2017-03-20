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

enum ObjectType {
	OT_EMPTY,
	OT_CUBE
};

class Scene
{
public:
	Scene();
	~Scene();

	void AddGameObject(ObjectType type);
	void DeleteGameObject(std::string name);
	GameObject* GetGameObject(std::string name);
	void LoadLevel(const char* path, const char* file);
	void RecursiveNodeRead(GameObject* go, aiNode& aiNode, GameObject* parentGO);

	void Draw();
	bool CleanUp();

	void DrawHierarchyNodes(GameObject* go);
	void DrawRecursively(GameObject* go);

private:
	std::list<GameObject *> gameobjects;
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