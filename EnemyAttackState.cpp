#include "EnemyAttackState.h"
#include "Timer.h"
#include "Enemy.h"
#include <ctime>
#include "Animation.h"
#include "EnemyIdleState.h"
#include "ModuleCollision.h"
#include "Application.h"
#include "Attack.h"

EnemyAttackState::EnemyAttackState() : EnemyStateMachine(ENEMY_ATTACKING) {
}


EnemyAttackState::~EnemyAttackState() {
}

void EnemyAttackState::Start(Enemy *enemy) {
	int number = rand() % enemy->attacks.size();
	Attack attack = enemy->attacks.at(number);
	enemy->setCurrentAnimation(attack.name);
	if (enemy->attackCollider != nullptr) {
		enemy->attackCollider->to_delete = true;
	}
	int attack_x = enemy->positionCollider->rect.x + (enemy->flipped ? -(enemy->positionCollider->rect.w / 2) : (enemy->positionCollider->rect.w / 2));

	enemy->attackCollider = App->collision->AddCollider({ attack_x, enemy->positionCollider->rect.y - attack.y, attack.x, enemy->positionCollider->rect.h + attack.y * 2 }, COLLIDER_TYPE::ENEMY_HIT, false, false, std::bind(&Enemy::OnCollision, enemy, std::placeholders::_1), enemy);
	enemy->setCurrentFx("attack");
}

EnemyStateMachine * EnemyAttackState::Update(Enemy *enemy) {
	distanceVector = enemy->distanceToTarget();
	flipEnemy(enemy);

	if (enemy->getCurrentAnimation()->Finished())
		return new EnemyIdleState();

	return nullptr;
}