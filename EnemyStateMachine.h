#ifndef ENEMY_STATE_MACHINE_H
#define ENEMY_STATE_MACHINE_H

#include "Point.h"

enum ENEMY_STATE {
	ENEMY_IDLE,
	ENEMY_MOVING,
	ENEMY_ATTACKING,
	ENEMY_DAMAGED,
	ENEMY_KILLED,
	ENEMY_GRABBED
};

class Enemy;
class Timer;

class EnemyStateMachine {
public:
	EnemyStateMachine(ENEMY_STATE state);
	virtual ~EnemyStateMachine();

	virtual void Start(Enemy *enemy) = 0;
	virtual EnemyStateMachine * Update(Enemy *enemy) = 0;
	virtual EnemyStateMachine * ChangeTo(ENEMY_STATE enemyState);

protected:
	ENEMY_STATE state;
	iPoint distanceVector{ 0,0,0 };
	Timer *timer;

protected:
	virtual void flipEnemy(Enemy *enemy);
};

#endif // !ENEMY_STATE_MACHINE_H

