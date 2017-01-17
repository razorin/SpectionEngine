#include "Creature.h"
#include "SDL/include/SDL.h"
#include "ModuleCollision.h"
#include "Animation.h"
#include "ModuleAudio.h"
#include "Application.h"

Creature::Creature(const JSON_Object *creatureConfig, ENTITY_TYPE type) : Entity(type) {
	life = (int)json_object_dotget_number(creatureConfig, "life");
	max_life = life;
	attack = (int)json_object_dotget_number(creatureConfig, "attack");
	lives = (int)json_object_dotget_number(creatureConfig, "lives");
	baseSpeed = (int)json_object_dotget_number(creatureConfig, "baseSpeed");

	JSON_Array *configAttacks = json_object_dotget_array(creatureConfig, "attacks");

	for (int i = 0; i < json_array_get_count(configAttacks); ++i) {
		JSON_Object *configAttack = json_array_get_object(configAttacks, i);
		Attack attack;
		attack.name = json_object_dotget_string(configAttack, "name");
		attack.x = (int)json_object_dotget_number(configAttack, "x");
		attack.y = (int)json_object_dotget_number(configAttack, "y");
		attacks.push_back(attack);
		icon = new Icon({ (int)json_object_dotget_number(creatureConfig, "icon.x"), (int)json_object_dotget_number(creatureConfig, "icon.y"),
			(int)json_object_dotget_number(creatureConfig, "icon.width"), (int)json_object_dotget_number(creatureConfig, "icon.height") },
			{ (int)json_object_dotget_number(creatureConfig, "icon.position.x"), (int)json_object_dotget_number(creatureConfig, "icon.position.y") });
	}
	
	JSON_Array *configAnimations = json_object_dotget_array(creatureConfig, "animations");


	for (int i = 0; i < json_array_get_count(configAnimations); ++i) {
		JSON_Object *configAnimation = json_array_get_object(configAnimations, i);
		std::string key = json_object_dotget_string(configAnimation, "name");

		Animation *anim = new Animation();
		anim->loop = json_object_dotget_boolean(configAnimation, "loop");
		anim->speed = json_object_dotget_number(configAnimation, "speed");

		JSON_Array *configFrames = json_object_dotget_array(configAnimation, "frames");

		for (int j = 0; j < json_array_get_count(configFrames); ++j) {
			JSON_Object *configFrame = json_array_get_object(configFrames, j);
			anim->frames.push_back(Frame({ (int)json_object_dotget_number(configFrame, "x"), (int)json_object_dotget_number(configFrame, "y"),
				(int)json_object_dotget_number(configFrame, "width"), (int)json_object_dotget_number(configFrame, "height") },
				(int)json_object_dotget_number(configFrame, "offset_x"), (int)json_object_dotget_number(configFrame, "offset_y")));
		}

		animations[key] = anim;
	}

	JSON_Array *configSounds = json_object_dotget_array(creatureConfig, "sounds");

	for (int i = 0; i < json_array_get_count(configSounds); ++i) {
		JSON_Object *configSound = json_array_get_object(configSounds, i);

		std::string key = json_object_dotget_string(configSound, "name");
		sounds[key] = App->audio->LoadFx(json_object_dotget_string(configSound, "path"));
	}
}

Creature::Creature(Creature const *other) : baseSpeed(other->baseSpeed), 
	lives(other->lives), max_life(other->life), attack(other->attack),
	life(other->life), Entity(other->type), attacks(other->attacks), icon(other->icon)
{
	for (auto it = other->animations.begin(); it != other->animations.end(); ++it) {
		Animation *anim = new Animation((*it->second));
		animations[(it->first)] = anim;
	}

	sounds = other->sounds;
}


Creature::~Creature() {
}

void Creature::Move(const iPoint &movement) {
	*position += movement;
	positionCollider->AddPoint(movement);
}

Icon * Creature::getIcon() const {
	return icon;
}