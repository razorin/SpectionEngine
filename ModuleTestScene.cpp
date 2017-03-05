#include "Globals.h"
#include "Application.h"
#include "ModuleTestScene.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePrimitives.h"
#include "Animation.h"
#include "Model.h"
#include "SCube.h"
#include "MemLeaks.h"


ModuleTestScene::ModuleTestScene(const JSON_Value *json, bool active) : Module(json, active) 
{
}

ModuleTestScene::ModuleTestScene(const bool active) : Module(active) 
{
}

ModuleTestScene::~ModuleTestScene()
{
}

bool ModuleTestScene::Start() {

	SPrimitive *primitive = nullptr;
	primitive = App->primitives->AddPrimitive(SPRIMITIVE_TYPE::SCUBE_TYPE, { 0, 6.5, 0 }, 0.25);
	primitives.push_back(primitive);
	primitive = App->primitives->AddPrimitive(SPRIMITIVE_TYPE::SCYLINDER_TYPE, { 0, 5, 0 }, 0.25);
	primitives.push_back(primitive);
	//model = new Model();
	//model->Load("models/batman/","batman.obj");

	//model2 = new Model();
	//model2->Load("Models/Magnetto/", "magnetto2.fbx");
	
	


	importedLevel = new Level();
	//importedLevel->Load("Models/street/", "Street.obj");
	importedLevel->Load("Models/street/", "Street environment_V01.fbx");

	//This is a little hack to se the scene in the correct rotation (Node Dummy001 is rotated -90 in X axis)
	float angleRad = 90 * DEGTORAD;
	//importedLevel->root->rotation = Quat::RotateX(angleRad);
	//importedLevel->RecursiveCalcTransforms(importedLevel->root);

	//Test FindNode function
	//Node* node = importedLevel->FindNode("g City_building_004");
	//if (node == nullptr) {
	//	DLOG("Node not found");
	//}
	//else
	//{
	//	DLOG("The name of the node is: %s", node->name.c_str());
	//}


	//This is a test to try the LinkNode and FindNode methods
	importedLevel->LinkNode(importedLevel->FindNode("City_building_014"), importedLevel->FindNode("City_building_001"));

	//importedLevel->FindNode("City_building_001")->rotation = Quat::RotateX(angleRad/2);
	//importedLevel->FindNode("g City_building_001")->position.y += 5;
	//importedLevel->RecursiveCalcTransforms(importedLevel->root);
	//importedLevel->FindNode("g City_building_014");
	//importedLevel->FindNode("g City_building_001");


	return true;
}

update_status ModuleTestScene::PreUpdate() 
{
	return UPDATE_CONTINUE;
}

update_status ModuleTestScene::Update(float dt) 
{
	return UPDATE_CONTINUE;
}

update_status ModuleTestScene::PostUpdate() 
{
	return UPDATE_CONTINUE;
}

void ModuleTestScene::Draw()
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

bool ModuleTestScene::CleanUp() {

	primitives.clear();

	if (model != nullptr)
	{
		model->Clear();
		RELEASE(model);
	}

	if (model2 != nullptr)
	{
		model2->Clear();
		RELEASE(model2);
	}

	if (importedLevel != nullptr)
	{
		importedLevel->Clear();
		RELEASE(importedLevel);
	}

	return true;
}