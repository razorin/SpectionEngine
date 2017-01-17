#ifndef ENEMY_H
#define ENEMY_H

#include "Creature.h"
#include "Parson.h"
#include "Point.h"
#include "Parson.h"
#include <vector>

class EnemyStateMachine;
struct Collider;
class Player;


enum  ENEMY_TYPE {
	BRED,
	SIMONS,
	JAKE,
	DUG,
	UNKNOWN_ENEMY
};

class Enemy : public Creature {
public:
	Enemy(const JSON_Object *enemyConfig, ENEMY_TYPE type);
	Enemy(const Enemy *other);
	virtual void Init(const iPoint &initialPosition) = 0;
	~Enemy();
	iPoint &distanceToTarget() const;
	void OnCollision(const Collider &other);
	void TakeDamage(int damage);
	void TakeDamage(Player *player);
	void Kill();
	void Update();

protected:
	iPoint previousPosition = { 0,0,0 };
	EnemyStateMachine *state = nullptr;
	iPoint targetPoint{ 0,0,0 };

public:
	ENEMY_TYPE type;
	int baseSpeed = 1;
	Collider *attackCollider = nullptr;
	Player *player = nullptr;
};

#endif // !ENEMY_H

