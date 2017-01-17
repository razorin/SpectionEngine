#include "Globals.h"
#include "Application.h"
#include "ModuleEntity.h"
#include "ModuleTextures.h"
#include "Entity.h"
#include "Player.h"
#include "Section.h"
#include "ModuleRender.h"
#include "Point.h"
#include "Enemy.h"
#include "Bred.h"
#include "Simons.h"
#include "Jake.h"
#include "Dug.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"

ModuleEntity::ModuleEntity(const JSON_Value *json, bool enable) : Module(json, enable) {
	config = json_value_get_object(json);
}


ModuleEntity::~ModuleEntity() {
	RELEASE(enemy);
}

Entity* ModuleEntity::Create(const ENTITY_TYPE &type) {
	static_assert(ENTITY_TYPE::UNKNOWN == 2, "Update entity types");
	Entity *result = nullptr;

	switch (type) {
		case ENTITY_TYPE::PLAYER: 
			//json_object_dotget_object(root, "config.entities")
			result = new Player(json_object_dotget_object(config, "player"));
			break;
	}

	if (result != nullptr) {
		entities.push_back(result);
		entities.sort([](const Entity *a, const Entity *b) { return a->positionCollider->rect.y + a->positionCollider->rect.h > b->positionCollider->rect.y + b->positionCollider->rect.h;});
	}

	return result;
}

Enemy* ModuleEntity::Create(const ENEMY_TYPE &type) {
	static_assert(ENEMY_TYPE::UNKNOWN_ENEMY == 4, "Update enemy types");
	Enemy *result = nullptr;

	switch (type) {
	case ENEMY_TYPE::BRED :
		result = new Bred(enemy);
		break;
	case ENEMY_TYPE::SIMONS:
		result = new Simons(simons);
		break;
	case ENEMY_TYPE::JAKE:
		result = new Jake(jake);
		break;
	case ENEMY_TYPE::DUG:
		result = new Dug(dug);
		break;
	}

	if (result != nullptr) {
		entities.push_back(result);
		entities.sort([](const Entity *a, const Entity *b) { return a->position->z < b->position->z;});
	}

	return result;
}

bool ModuleEntity::Start() {
	LOG("Started Module Entity");
	enemy = new Bred(json_object_dotget_object(config, "enemies.bred"));
	simons = new Simons(json_object_dotget_object(config, "enemies.simons"));
	jake = new Jake(json_object_dotget_object(config, "enemies.jake"));
	dug = new Dug(json_object_dotget_object(config, "enemies.dug"));

	return true;
}

update_status ModuleEntity::PreUpdate() {
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end();) {
		if ((*it)->to_delete == true) {
			RELEASE(*it);
			it = entities.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEntity::Update() {
	entities.sort([](const Entity *a, const Entity *b) { return a->positionCollider->rect.y + a->positionCollider->rect.h < b->positionCollider->rect.y + b->positionCollider->rect.h;});
	std::list<Entity*>::iterator it;
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		if ((*it)->active) {
			(*it)->Update();
			if (&(*it)->getCurrentFrame() != nullptr)
				App->renderer->Blit((*it)->getGraphics(), *(*it)->position, &(*it)->getCurrentFrame(), (*it)->flipped);
			unsigned int fx = (*it)->getCurrentFx();
			if ( fx != -1)
				App->audio->PlayFx(fx);
		}
		
	}
	return update_status::UPDATE_CONTINUE;
}

bool ModuleEntity::CleanUp() {
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		RELEASE(*it);

	entities.clear();
	return true;
}
