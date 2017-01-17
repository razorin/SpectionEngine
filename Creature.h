#ifndef CREATURE_H
#define CREATURE_H

#include "Entity.h"
#include "Point.h"
#include "Parson.h"
#include <vector>
#include "Attack.h"

struct SDL_Texture;
struct Icon;


class Creature : public Entity {
public:
	Creature(const JSON_Object *playerConfig, ENTITY_TYPE type);
	Creature(Creature const *other);
	~Creature();
	void Move(const iPoint &movement);
	Icon * getIcon() const;

public:
	int max_life = 0;
	int life = 0;
	int lives = 0;
	int attack = 0;
	iPoint speed{ 0,0,0 };
	int baseSpeed = 0;
	std::vector<Attack> attacks;

protected:
	Icon *icon = nullptr;

};

#endif // !CREATURE_H

