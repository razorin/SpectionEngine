#ifndef ENEMY_KILLED_STATE_H
#define ENEMY_KILLED_STATE_H

#include "EnemyStateMachine.h"

class Enemy;

class EnemyKilledState :
	public EnemyStateMachine {
public:
	EnemyKilledState();
	~EnemyKilledState();
	virtual void Start(Enemy *enemy);
	virtual EnemyStateMachine * Update(Enemy *enemy);
	virtual EnemyStateMachine * ChangeTo(ENEMY_STATE enemyState);
};

#endif // !ENEMY_KILLED_STATE_H


