#include "EnemyKilledState.h"
#include "EnemyIdleState.h"
#include "Enemy.h"
#include "Animation.h"
#include "ModuleCollision.h"


EnemyKilledState::EnemyKilledState() : EnemyStateMachine(ENEMY_KILLED){
}


EnemyKilledState::~EnemyKilledState() {
}

void EnemyKilledState::Start(Enemy *enemy) {
	const static std::string anim = "killed";
	--enemy->lives;
	enemy->setCurrentAnimation(anim);
	enemy->setCurrentFx("killed");
}

EnemyStateMachine * EnemyKilledState::Update(Enemy *enemy) {
	if (enemy->getCurrentAnimation()->Finished()) {
		if (enemy->lives <= 0) {
			enemy->positionCollider->to_delete = true;
			enemy->to_delete = true;
		}
		else
			return new EnemyIdleState();
	}
	return nullptr;
}

EnemyStateMachine * EnemyKilledState::ChangeTo(ENEMY_STATE enemyState) {
	return nullptr;
}
