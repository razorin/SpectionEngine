#include "EnemyStateMachine.h"
#include "EnemyIdleState.h"
#include "EnemyDamageState.h"
#include "Point.h"
#include "Enemy.h"
#include "Player.h"
#include "Timer.h"

EnemyStateMachine::EnemyStateMachine(ENEMY_STATE state) : state(state) {
	timer = new Timer();
}

EnemyStateMachine::~EnemyStateMachine() {
	RELEASE(timer);
}

EnemyStateMachine * EnemyStateMachine::ChangeTo(ENEMY_STATE enemyState) {
	if (state == enemyState)
		return nullptr;
	switch (enemyState) {
	case ENEMY_IDLE:
		return new EnemyIdleState();
		break;
	case ENEMY_DAMAGED:
		return new EnemyDamageState();
	}
}

void EnemyStateMachine::flipEnemy(Enemy *enemy) {
	if (distanceVector.x < 0) {
		enemy->flipped = true;
	}
	else {
		enemy->flipped = false;
	}
}

