#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <map>

template <class TYPE> class Point;
typedef Point<int> iPoint;

struct SDL_Texture;
struct SDL_Rect;
struct Collider;
struct Frame;
class Animation;

enum ENTITY_TYPE {
	PLAYER,
	ENEMY,
	UNKNOWN
};

class Entity {
public:
	Entity(ENTITY_TYPE type);
	virtual ~Entity();
	virtual void Update();
	SDL_Texture* getGraphics() const;
	Frame & getCurrentFrame() const;
	void setCurrentAnimation(const std::string &animation);
	void setCurrentFx(const std::string fx);
	unsigned int  getCurrentFx();
	Animation *getCurrentAnimation() const;
	iPoint &getBottomPoint() const;

public:
	std::list<Entity *> entities;
	Collider* positionCollider = nullptr;
	bool to_delete = false;
	ENTITY_TYPE type;
	iPoint* position = nullptr;
	bool flipped = false;
	std::map<std::string, Animation*> animations;
	std::map<std::string, unsigned int > sounds;

	bool active = false;

protected:
	SDL_Texture* graphics = nullptr;
	//SDL_Rect *section = nullptr;
	Animation *currentAnimation = nullptr;
	int currentFx = -1;
};

#endif // !ENTITY_H

