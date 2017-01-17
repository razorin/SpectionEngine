#include "EnemyDamageState.h"
#include <string>
#include "Enemy.h"
#include "Animation.h"
#include "EnemyIdleState.h"
#include "EnemyKilledState.h"

EnemyDamageState::EnemyDamageState() : EnemyStateMachine(ENEMY_DAMAGED){
}


EnemyDamageState::~EnemyDamageState() {
}

void EnemyDamageState::Start(Enemy *enemy) {
	const static std::string anim = "damage";
	enemy->setCurrentAnimation(anim);
	//timer->Start();
	enemy->setCurrentFx("damage");
}

EnemyStateMachine * EnemyDamageState::Update(Enemy *enemy) {
	if (enemy->getCurrentAnimation()->Finished()) {
		if (enemy->life <= 0)
			return new EnemyKilledState();
		else
			return new EnemyIdleState();
	}
	return nullptr;
}