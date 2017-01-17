#include "EnemyMoveState.h"
#include <random>
#include "Enemy.h"
#include "Player.h"
#include "EnemyIdleState.h"
#include "ModuleCollision.h"
#include "Point.h"
#include "Timer.h"
#include "EnemyAttackState.h"

EnemyMoveState::EnemyMoveState() : EnemyStateMachine(ENEMY_MOVING) {
}


EnemyMoveState::~EnemyMoveState() {
}


void EnemyMoveState::Start(Enemy *enemy) {
	const static std::string anim = "movement";
	enemy->setCurrentAnimation(anim);
	timer->Start();
}

EnemyStateMachine * EnemyMoveState::Update(Enemy *enemy) {
	distanceVector = enemy->distanceToTarget();
	flipEnemy(enemy);

	int number = rand() % 10000;
	if (distanceVector.x <= 40 && distanceVector.x >= -40 &&
		distanceVector.y <= 10 && distanceVector.y >= -10) {
		//if (number > 7000)
			return new EnemyAttackState();
		/*else
			return new EnemyIdleState();*/
	}

	if (timer->Ellapsed() >= 3000) {
			return new EnemyIdleState();
	}


	if (!distanceVector.IsZero()) {
		if (distanceVector.x != 0 && distanceVector.y != 0) {
			if(number > 2500)
				enemy->speed.x = distanceVector.x < 0 ? -enemy->baseSpeed : distanceVector.x > 0 ? enemy->baseSpeed : 0;
			else
				enemy->speed.y = distanceVector.y < 0 ? -enemy->baseSpeed : distanceVector.y > 0 ? enemy->baseSpeed : 0;
		}
		else if (distanceVector.x != 0) {
			enemy->speed.x = distanceVector.x < 0 ? -enemy->baseSpeed : distanceVector.x > 0 ? enemy->baseSpeed : 0;
		} else if(distanceVector.y != 0){
			enemy->speed.y = distanceVector.y < 0 ? -enemy->baseSpeed : distanceVector.y > 0 ? enemy->baseSpeed : 0;

		}
	} 

	return nullptr;	
}
