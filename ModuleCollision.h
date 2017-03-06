#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include <functional>
#include "Module.h"
#include "Point.h"
#include "MemLeaks.h"

enum COLLIDER_TYPE {
	NONE,
	PLAYER_COLLIDER,
	PLAYER_HIT,
	ENEMY_COLLIDER,
	ENEMY_HIT,
	WALL,
	ITEM_DESTROYABLE,
	ITEM_PICKABLE,
	CAMERA_WALL
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	int z = 0;
	bool ignore_z = false;
	bool ignore_y = false;
	COLLIDER_TYPE type;
	std::function<void(const Collider &)> onCollision;


	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, bool ignore_z, bool ignore_y, std::function<void(const Collider &)> onCollision) : // expand this call if you need to
		rect(rectangle), z(0), type(type), ignore_z(ignore_z), ignore_y(ignore_y), onCollision(onCollision)
	{

	}

	void SetPos(int x, int y, int z)
	{
		rect.x = x;
		rect.y = y;
		this->z = z;
	}

	void AddPoint(const iPoint &movement) {
		rect.x += movement.x;
		rect.y += movement.y;
		this->z += movement.z;
	}

	bool CheckCollision(const Collider &other) const;

	void Notify(const Collider &other);
};

class ModuleCollision : public Module
{
public:

	ModuleCollision(const JSON_Object *json = nullptr);
	~ModuleCollision();

	update_status PreUpdate(float dt);
	update_status Update(float dt);

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect, COLLIDER_TYPE type = COLLIDER_TYPE::NONE, bool ignore_z = false, bool ignore_y = false, std::function<void(const Collider &)> onCollision = nullptr);
	void DebugDraw();

private:
	bool *matrix_collision;
	std::list<Collider*> colliders;
	bool debug = false;
};

#endif // __ModuleCollision_H__