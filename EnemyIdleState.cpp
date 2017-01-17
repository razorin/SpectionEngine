#include "EnemyIdleState.h"
#include "Enemy.h"
#include "Player.h"
#include "ModuleCollision.h"
#include <random>
#include "EnemyMoveState.h"
#include "Timer.h"
#include "EnemyAttackState.h"

EnemyIdleState::EnemyIdleState() : EnemyStateMachine(ENEMY_IDLE){
}


EnemyIdleState::~EnemyIdleState() {
}

void EnemyIdleState::Start(Enemy *enemy) {
	const static std::string anim = "idle";
	enemy->setCurrentAnimation(anim);
	timer->Start();

}

EnemyStateMachine * EnemyIdleState::Update(Enemy *enemy) {
	//return new EnemyAttackState();
	distanceVector = enemy->distanceToTarget();
	flipEnemy(enemy);

	int number = rand() % 10000;
	if (timer->Ellapsed() >= number/5) {


		if (number < 4500)
			return nullptr;
		else {
			return new EnemyMoveState();
		}
	}
	return nullptr;
}