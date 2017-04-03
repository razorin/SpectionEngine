#ifndef __MODULESCENEMANAGER_H__
#define __MODULESCENEMANAGER_H__

#include "Module.h"

class Scene;

class ModuleSceneManager : public Module
{
public:
	ModuleSceneManager(const JSON_Object *json, bool active);
	~ModuleSceneManager();

	void setCurrentScene(Scene* scene);
	Scene* getCurrentScene();
	void deleteCurrentScene();
	void loadCurrentScene(const char* path, const char* file);

	void Play();
	void Stop();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt = 0);
	update_status PostUpdate(float dt);

	void Draw();

	bool CleanUp();

private:
	Scene* currentScene = nullptr;
	Scene* backupScene = nullptr;
};

#endif // __MODULESCENEMANAGER_H__