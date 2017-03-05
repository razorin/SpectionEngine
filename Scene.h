#ifndef __SCENE_H__
#define __SCENE_H__

#include <list>

class GameObject;
class Model;
class Level;
class SPrimitive;
class SCube;
class SPlane;
class SCylinder;

class Scene
{
public:
	Scene();
	~Scene();

	void addGameObject(GameObject* gameobject) { gameobjects.push_back(gameobject); }
	GameObject* getGameObject(std::string name);
	void loadLevel(const char* path, const char* file);
	void Draw();

private:
	std::list<GameObject *> gameobjects;
	std::list<SPrimitive*> primitives;
	SCube *cube = nullptr;
	SPlane *plane = nullptr;
	SCylinder *cylinder = nullptr;
	Model *model = nullptr;
	Model *model2 = nullptr;
	Level* importedLevel = nullptr;
};

#endif // __SCENE_H__