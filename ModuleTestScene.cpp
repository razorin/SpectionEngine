#include "Globals.h"
#include "Application.h"
#include "ModuleTestScene.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Animation.h"
#include "SCube.h"

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
}

bool ModuleTestScene::CleanUp() {

	for (std::list<SPrimitive*>::iterator it = primitives.begin(); it != primitives.end(); ++it)
	{
		RELEASE(*it);
	}
	return true;
}