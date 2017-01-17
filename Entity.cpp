#include "Entity.h"
#include "Point.h"
#include "Globals.h"
#include <string.h>
#include "Animation.h"
#include <assert.h>
#include "ModuleCollision.h"

Entity::Entity(ENTITY_TYPE type) : type(type) {
	position = new iPoint(0, 0, 0);

}


Entity::~Entity() {
	RELEASE(position);
	graphics = nullptr;
	
	for (auto it = animations.begin(); it != animations.end(); ++it) {
		RELEASE(it->second);
	}
	animations.clear();

	entities.clear();
}

void Entity::Update() {
	LOG("Updating entity");
}

SDL_Texture* Entity::getGraphics() const {
	return graphics;
}


Frame & Entity::getCurrentFrame() const {
	return currentAnimation->GetCurrentFrame();
}

void Entity::setCurrentAnimation(const std::string &animation) {
	map<std::string, Animation*>::iterator anim = animations.find(animation);
	if (anim != animations.end())
		this->currentAnimation = anim->second;
	else
		this->currentAnimation = nullptr;
}

Animation * Entity::getCurrentAnimation() const {
	return currentAnimation;
}

iPoint &Entity::getBottomPoint() const {
	if (positionCollider == nullptr)
		return iPoint{ 0,0,0 };
	return iPoint{ positionCollider->rect.x + positionCollider->rect.w / 2,
		positionCollider->rect.y + positionCollider->rect.h, 0 };
}

void Entity::setCurrentFx(const std::string fx) {
	map<std::string, unsigned int>::iterator sound = sounds.find(fx);
	if (sound != sounds.end())
		this->currentFx = sound->second;
	else
		this->currentFx = -1;
}

unsigned int  Entity::getCurrentFx(){
	unsigned int  aux = currentFx;
	currentFx = -1;
	return aux;
}