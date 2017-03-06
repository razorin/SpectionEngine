#include "ModuleSceneManager.h"
#include "Scene.h"


ModuleSceneManager::ModuleSceneManager(const JSON_Object *json, bool active) : Module(json,active)
{
}


ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Start() {
	loadCurrentScene("Models/street/", "Street environment_V01.fbx");
	
	return true;
}

update_status ModuleSceneManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneManager::Update(float dt)
{
	//TODO this should be done in moduleRender update, NOT HERE
	if (currentScene != nullptr) currentScene->Draw();
	return UPDATE_CONTINUE;
}

update_status ModuleSceneManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleSceneManager::CleanUp()
{
	//TODO Clean up
	return true;
}

void ModuleSceneManager::setCurrentScene(Scene * scene)
{
	if (currentScene == nullptr) {
		currentScene = scene;
	}
	else {
		DLOG("There's a currentScene already!");
	}
}

Scene * ModuleSceneManager::getCurrentScene()
{
	return currentScene;
}

void ModuleSceneManager::deleteCurrentScene()
{
	RELEASE(currentScene);
	currentScene = nullptr;
}

void ModuleSceneManager::loadCurrentScene(const char * path, const char * file)
{
	if (currentScene != nullptr) {
		currentScene->loadLevel(path,file);
	}
	else {
		Scene* scene = new Scene();
		currentScene = scene;
		currentScene->loadLevel(path, file);
	}
}
