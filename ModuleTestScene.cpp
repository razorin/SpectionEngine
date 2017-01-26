#include "ModuleTestScene.h"
#include "Animation.h"

ModuleTestScene::ModuleTestScene(const JSON_Value *json, bool active) : Module(json, active) {
	
}

ModuleTestScene::ModuleTestScene(const bool active) : Module(active) {
	
	
}


ModuleTestScene::~ModuleTestScene() {
}

bool ModuleTestScene::Start() {
	graphics = App->textures->Load("yusuke.png");

	frame = new Frame({ 14,76,17,36 });

	position.x = 100;
	position.y = 100;

	return true;
}

update_status ModuleTestScene::PreUpdate() {

	return UPDATE_CONTINUE;

}

update_status ModuleTestScene::Update() {

	
	return UPDATE_CONTINUE;
}

update_status ModuleTestScene::PostUpdate() {
	DLOG("Drawing Sprite");
	App->renderer->Blit(graphics, position, frame, false, 1);
	return UPDATE_CONTINUE;
}

bool ModuleTestScene::CleanUp() {
	RELEASE(frame);
	//RELEASE(graphics);
	return true;
}