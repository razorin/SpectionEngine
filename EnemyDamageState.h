#ifndef ENEMY_DAMAGE_STATE_H
#define ENEMY_DAMAGE_STATE_H

#include "EnemyStateMachine.h"

class Enemy;

class EnemyDamageState : public EnemyStateMachine {
public:
	EnemyDamageState();
	~EnemyDamageState();
	virtual void Start(Enemy *enemy);
	virtual EnemyStateMachine * Update(Enemy *enemy);
};

#endif // !ENEMY_DAMAGE_STATE_H

