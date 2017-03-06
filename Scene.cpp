#include "Scene.h"
#include "GameObject.h"
#include "SCube.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePrimitives.h"
#include "SPrimitive.h"
#include "Level.h"
#include "Model.h"


Scene::Scene()
{
}


Scene::~Scene()
{
	if(cube != nullptr) RELEASE(cube);
	if (plane != nullptr) RELEASE(plane);
	if (cylinder != nullptr) RELEASE(cylinder);
	if (model != nullptr) {
		model->Clear();
		RELEASE(model);
	}
	if (model2 != nullptr) {
		model2->Clear();
		RELEASE(model2);
	}
	if (importedLevel != nullptr) {
		importedLevel->Clear();
		RELEASE(importedLevel);
	}

	gameobjects.clear();
	primitives.clear();

}

GameObject * Scene::getGameObject(std::string name)
{
	bool found = false;
	GameObject* gameobject;
	for (std::list<GameObject *>::iterator it = gameobjects.begin(); it != gameobjects.end();) {
		if((*it)->name.compare(name) == 0) {
			gameobject = *it;
			found = true;
		}
		else {
			++it;
		}
	}
	if (found) {
		return gameobject;
	}
	else {
		return nullptr;
	}
}

void Scene::loadLevel(const char * path, const char * file)
{
	/*SPrimitive *primitive = nullptr;
	primitive = App->primitives->AddPrimitive(SPRIMITIVE_TYPE::SCUBE_TYPE, { 0, 6.5, 0 }, 0.25);
	primitives.push_back(primitive);
	primitive = App->primitives->AddPrimitive(SPRIMITIVE_TYPE::SCYLINDER_TYPE, { 0, 5, 0 }, 0.25);
	primitives.push_back(primitive);*/
	//model = new Model();
	//model->Load("models/batman/","batman.obj");

	//model2 = new Model();
	//model2->Load("Models/Magnetto/", "magnetto2.fbx");




	importedLevel = new Level();
	//importedLevel->Load("Models/street/", "Street.obj");
	importedLevel->Load("Models/street/", "Street environment_V01.fbx");

	//This is a little hack to se the scene in the correct rotation (Node Dummy001 is rotated -90 in X axis)
	float angleRad = 90 * DEGTORAD;

	//This is a test to try the LinkNode and FindNode methods
	importedLevel->LinkNode(importedLevel->FindNode("City_building_014"), importedLevel->FindNode("City_building_001"));
}

void Scene::Draw()
{
	for (std::list<SPrimitive*>::iterator it = primitives.begin(); it != primitives.end(); ++it)
	{
		(*it)->Draw();
	}

	if (model != nullptr)
		model->Draw();

	if (model2 != nullptr)
		model2->Draw();



	if (importedLevel != nullptr)
	{
		importedLevel->Draw(importedLevel->root);
	}
}
