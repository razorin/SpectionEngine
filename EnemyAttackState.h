#ifndef ENEMY_ATTACK_STATE_H
#define ENEMY_ATTACK_STATE_H

#include "EnemyStateMachine.h"

class Enemy;

class EnemyAttackState : public EnemyStateMachine {
public:
	EnemyAttackState();
	~EnemyAttackState();
	virtual void Start(Enemy *enemy);
	virtual EnemyStateMachine * Update(Enemy *enemy);
};

#endif // !ENEMY_ATTACK_STATE_H

