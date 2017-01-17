#ifndef ENEMY_IDLE_STATE_H
#define ENEMY_IDLE_STATE_H

#include "EnemyStateMachine.h"

class Enemy;

class EnemyIdleState : public EnemyStateMachine {
public:
	EnemyIdleState();
	~EnemyIdleState();

	virtual void Start(Enemy *enemy);
	virtual EnemyStateMachine * Update(Enemy *enemy);
};

#endif // !ENEMY_IDLE_STATE_H

