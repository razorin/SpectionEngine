#include "Globals.h"
#include "Application.h"
#include "ModuleTestScene.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Animation.h"
#include "Model.h"
#include "SCube.h"
#include "SCylinder.h"
#include "SPlane.h"


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

	primitives.push_back(cube = new SCube());
	model = new Model();
	model->Load("Models/Batman/","Batman.obj");
	model2 = new Model();
	model2->Load("Models/Magnetto/", "magnetto2.fbx");
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
}

bool ModuleTestScene::CleanUp() {

	for (std::list<SPrimitive*>::iterator it = primitives.begin(); it != primitives.end(); ++it)
	{
		RELEASE(*it);
	}

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

	return true;
}