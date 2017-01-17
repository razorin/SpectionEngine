#include "Simons.h"
#include "Animation.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Player.h"
#include "EnemyStateMachine.h"


int Simons::number_of_instances = 0;

Simons::Simons(const JSON_Object *bredConfig) : Enemy(bredConfig, ENEMY_TYPE::SIMONS) {
	const char* path = json_object_dotget_string(bredConfig, "graphics");
	graphics = App->textures->Load(path);

	currentAnimation = animations["idle"];

	++number_of_instances;
}

Simons::Simons(const Simons *other) : Enemy(other) {
	graphics = other->graphics;

	currentAnimation = animations["idle"];
	position = new iPoint(*other->position);
	positionCollider = App->collision->AddCollider({ position->x, position->y, 37, 88 }, COLLIDER_TYPE::ENEMY_COLLIDER, false, false, std::bind(&Simons::OnCollision, this, std::placeholders::_1));
	++number_of_instances;
}


Simons::~Simons() {
	if (--number_of_instances > 0)
		graphics = nullptr;
	currentAnimation = nullptr;
}



void Simons::Init(const iPoint &initialPosition) {
	active = true;
	*position = initialPosition;
	positionCollider->SetPos(position->x + 40, position->y + 15, position->z);
}