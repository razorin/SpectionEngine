#ifndef MODULE_ENTITY_H
#define MODULE_ENTITY_H

#include <list>
#include "Module.h"
#include "Parson.h"

class Entity;
enum ENTITY_TYPE;
enum ENEMY_TYPE;
class Bred;
class Simons;
class Jake;
class Dug;
class Enemy;

class ModuleEntity : public Module {
public:
	ModuleEntity(const JSON_Value *json, bool enable);
	~ModuleEntity();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	Entity* Create(const ENTITY_TYPE &type);
	Enemy* Create(const ENEMY_TYPE &type);

private:
	JSON_Object *config = nullptr;
	std::list<Entity*> entities;
	Bred *enemy = nullptr;
	Simons *simons = nullptr;
	Jake *jake = nullptr;
	Dug *dug = nullptr;

};

#endif // !MODULE_ENTITY_H

