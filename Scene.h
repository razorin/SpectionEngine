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
	Scene(const Scene* scene);
	~Scene();

	GameObject* AddGameObject(GameObject* parent = nullptr, bool editable = false, const std::string &name = "");

	GameObject* GetGameObject(std::string code);
	void LoadLevel(const char* path, const char* file);
	void RecursiveNodeRead(GameObject* go, aiNode& aiNode, GameObject* parentGO);
	void BindBonesTransform();

	void Draw();
	void Update(float dt);
	bool CleanUp();

	void DrawHierarchyNodes(GameObject* go);
	void DrawRecursively(GameObject* go);


public:
	GameObject* root = nullptr;
	//std::list<GameObject *> gameobjects;

private:
	int gameObjectsCounter = 0;
	std::vector<Mesh *> meshes;
	std::vector<Material *> materials;
	//std::list<Animation *> animations;

	int frame = 0;
	int maxFrames = 0;

	//TODO use the texture manager instead of this
	uint* textureIds = 0;

};

#endif // __SCENE_H__