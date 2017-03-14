#include "ModuleSceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ComponentTransform.h"


ModuleSceneManager::ModuleSceneManager(const JSON_Object *json, bool active) : Module(json, active)
{
}


ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Start() {
	//loadCurrentScene("Models/street/", "Street environment_V01.fbx");
	loadCurrentScene("Models/ArmyPilot/", "ArmyPilot.dae");
	//loadCurrentScene("Models/Batman/", "Batman.obj");
	//loadCurrentScene("Models/street/", "Street.obj");
	//GameObject* go = currentScene->GetGameObject("Street.obj");
	


	currentScene->DebugGOInfo(currentScene->root);
	return true;
}

update_status ModuleSceneManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleSceneManager::Draw()
{
	if (currentScene != nullptr) {
		GameObject* go = currentScene->GetGameObject("RootNode");
		if (go != nullptr)
		{
			//go->transform->SetPosition(go->transform->Position() + float3(0.002, 0, 0));
			go->transform->Rotate(0.001f, float3::unitY);
		}
		currentScene->Draw();
	}
}

bool ModuleSceneManager::CleanUp()
{
	//TODO Clean up
	if (currentScene != nullptr)
	{
		currentScene->CleanUp();
		RELEASE(currentScene);
	}
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
	if (currentScene != nullptr) RELEASE(currentScene);
	currentScene = nullptr;
}

void ModuleSceneManager::loadCurrentScene(const char * path, const char * file)
{
	if (currentScene != nullptr) {
		currentScene->LoadLevel(path, file);
	}
	else {
		Scene* scene = new Scene();
		currentScene = scene;
		currentScene->LoadLevel(path, file);
	}
}
