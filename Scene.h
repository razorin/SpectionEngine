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

	void AddGameObject(GameObject* gameobject);
	GameObject* GetGameObject(std::string name);
	void LoadLevel(const char* path, const char* file);
	void RecursiveNodeRead(GameObject* go, aiNode& aiNode, GameObject* parentGO);

	void Draw();
	void DebugGOInfo(GameObject* go);
	GameObject* root;
	bool CleanUp();

	void DrawHierarchyNodes(GameObject* go);
	void TransformHierarchy();

private:
	std::list<GameObject *> gameobjects;
	std::vector<Mesh *> meshes;
	std::vector<Material *> materials;
	//std::list<Animation *> animations;

	int frame = 0;
	int maxFrames;

	//TODO use the texture manager instead of this
	uint* textureIds;
};

#endif // __SCENE_H__