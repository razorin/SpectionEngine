#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Point.h"
#include "ModuleCollision.h"

#include "ModuleLevelOneStageOne.h"
#include "ModuleEntity.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
#include "Player.h"
#include "Animation.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "Section.h"
#include "Enemy.h"

ModuleLevelOneStageOne::ModuleLevelOneStageOne(const JSON_Value *json, bool active) : Module(json, active) {
	config = json_value_get_object(json);
}


ModuleLevelOneStageOne::~ModuleLevelOneStageOne() {
	Clear();
}

bool ModuleLevelOneStageOne::Start() {
	LOG("Loading Level 1-1 ");
	player = (Player *)App->entities->Create(ENTITY_TYPE::PLAYER);
	player->Init(iPoint(16, 80, 0));

	const char* path = json_object_dotget_string(config, "graphics");
	graphics = App->textures->Load(path);

	scene = new Frame({ (int)json_object_dotget_number(config, "scene.x"),
		(int)json_object_dotget_number(config, "scene.y"),
		(int)json_object_dotget_number(config, "scene.width"),
		(int)json_object_dotget_number(config, "scene.height")
	});

	JSON_Array *configWalls = json_object_dotget_array(config, "walls");

	for (int i = 0; i < json_array_get_count(configWalls); ++i) {
		JSON_Object *configWall = json_array_get_object(configWalls, i);
		App->collision->AddCollider({ (int)json_object_dotget_number(configWall, "x"),
			(int)json_object_dotget_number(configWall, "y"),
			(int)json_object_dotget_number(configWall, "width"),
			(int)json_object_dotget_number(configWall, "height")
		}, COLLIDER_TYPE::WALL,
			(bool)json_object_dotget_boolean(configWall, "ignore_z"),
			(bool)json_object_dotget_boolean(configWall, "ignore_y"));
	}

	JSON_Array *configCameraWalls = json_object_dotget_array(config, "cameraWalls");

	for (int i = 0; i < json_array_get_count(configCameraWalls); ++i) {
		JSON_Object *configCameraWall = json_array_get_object(configCameraWalls, i);
		Collider * collider = App->collision->AddCollider({ (int)json_object_dotget_number(configCameraWall, "x"),
			(int)json_object_dotget_number(configCameraWall, "y"),
			(int)json_object_dotget_number(configCameraWall, "width"),
			(int)json_object_dotget_number(configCameraWall, "height")
		}, COLLIDER_TYPE::CAMERA_WALL, true, true);
		std::string name = json_object_dotget_string(configCameraWall, "name");
		cameraWalls[name] = collider;
	}

	JSON_Array *configSections = json_object_dotget_array(config, "sections");
	sections.reserve((int)json_array_get_count(configSections));

	for (int i = 0; i < json_array_get_count(configSections); ++i) {
		JSON_Object *configSection = json_array_get_object(configSections, i);
		sections.push_back(new Section((int)json_object_dotget_number(configSection, "endPoint"), 
			(int)json_object_dotget_number(configSection, "numberOfEnemies")));
	}

	currentSection = sections.at(sectionIndex);
	App->audio->PlayMusic("resources/music/level-1.wav");
	return true;
}

update_status ModuleLevelOneStageOne::PreUpdate() {
	if (player != nullptr && player->to_delete == true)
		player = nullptr;
	return UPDATE_CONTINUE;

}

update_status ModuleLevelOneStageOne::Update() {
	
	App->renderer->Blit(graphics, iPoint{ 0, 0 }, scene);
	//If current section is finished, increment section
	if (currentSection->IsFinished()){
		if (sectionIndex + 1 < sections.size() && cameraWalls["right"]->rect.x >= currentSection->section_ends) {
			++sectionIndex;
			currentSection = sections.at(sectionIndex);
			int numberOfEnemies = currentSection->getNumberOfEnemies();
			for (int i = 0; i < numberOfEnemies; ++i) {
				Enemy *enemy = nullptr;
				if (i % 4 == 0)
					enemy = App->entities->Create(ENEMY_TYPE::BRED);
				else if (i % 4 == 1)
					enemy = App->entities->Create(ENEMY_TYPE::SIMONS);
				else if (i % 4 == 2)
					enemy = App->entities->Create(ENEMY_TYPE::JAKE);
				else if (i % 4 == 3)
					enemy = App->entities->Create(ENEMY_TYPE::DUG);
				if (i % 2 == 0)
					enemy->Init(iPoint(cameraWalls["left"]->rect.x - 100 - i * 10, 80 + i * 2, 0));
				else
					enemy->Init(iPoint(cameraWalls["right"]->rect.x + 100 + i * 10, 80 + i * 2, 0));
				enemy->player = player;

				enemies.push_back(enemy);
			}
		}
		else if (sectionIndex == sections.size() - 1)
			App->fade->FadeToBlack(nullptr, this);
	}

	//Section *section = sections.at(currentSection);
	//Camera Movement
	if (player != nullptr) {
		int playerCenter = player->positionCollider->rect.x + player->positionCollider->rect.w / 2;
		int cameraCenter = (App->renderer->camera.w / 2 - App->renderer->camera.x) / App->window->screen_size;
		int movement = playerCenter - cameraCenter;

		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
			currentSection->EnemyKilled();
		}

		if (movement > 0) {
			bool sectionReached = cameraWalls["right"]->rect.x + 1 > currentSection->section_ends;
		
			if (!sectionReached) {
				App->renderer->camera.x -= (App->window->screen_size);
				cameraWalls["left"]->AddPoint(iPoint(1, 0));
				cameraWalls["right"]->AddPoint(iPoint(1, 0));
			}
		}
		
		App->renderer->DrawRect({ 25, 18, 125, 7 }, 255, 0, 0, 255);
		if (player != nullptr && player->life > 0) {
			App->renderer->StaticBlit(player->getGraphics(), player->getIcon()->position, player->getIcon()->section);
			App->renderer->DrawRect({ 25, 18, 125 * player->life/ player->max_life, 7 }, 247, 247, 43, 255);
		}
		if (player->currentEnemy != nullptr && player->currentEnemy->to_delete == false) {
			App->renderer->StaticBlit(player->currentEnemy->getGraphics(), player->currentEnemy->getIcon()->position, player->currentEnemy->getIcon()->section);
			App->renderer->DrawRect({ 212, 18, player->currentEnemy->max_life, 7 }, 255, 0, 0, 255);
			if(player->currentEnemy->life > 0)
				App->renderer->DrawRect({ 212, 18, player->currentEnemy->max_life * player->currentEnemy->life / player->currentEnemy->max_life, 7 }, 247, 247, 43, 255);
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleLevelOneStageOne::PostUpdate() {
	for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();) {
		if ((*it)->to_delete == true) {
			currentSection->EnemyKilled();
			it = enemies.erase(it);
		}
		else
			++it;
	}
	return UPDATE_CONTINUE;
}

bool ModuleLevelOneStageOne::CleanUp() {
	LOG("Unloading Level 1-1");
	Clear();
	return true;
}

void ModuleLevelOneStageOne::Clear() {
	for (auto it = cameraWalls.begin(); it != cameraWalls.end(); ++it) {
		it->second->to_delete = true;
	}
	cameraWalls.clear();
	for (int i = 0; i < sections.size(); ++i) {
		RELEASE(sections.at(i));
	}
	sections.clear();
	RELEASE(scene);
	//RELEASE(graphics);
	json_object_clear(config);
	RELEASE(config);
}