#include "CodyIdleJumpAttackState.h"
#include "CodyIdleFallState.h"
#include "ModuleCollision.h"
#include "Application.h"

CodyIdleJumpAttackState::CodyIdleJumpAttackState() {
}


CodyIdleJumpAttackState::~CodyIdleJumpAttackState() {
}

void CodyIdleJumpAttackState::Start(Player *player) {
	Attack attack;
	for (int i = 0; i < player->attacks.size(); i++) {
		if (player->attacks.at(i).name == "idleJumpAttack") {
			attack = player->attacks.at(i);
			break;
		}

	}
	player->setCurrentAnimation(attack.name);
	player->setCurrentFx("attack");
	if (player->attackCollider != nullptr) {
		player->attackCollider->to_delete = true;
	}
	int attack_x = player->positionCollider->rect.x + (player->flipped ? -(player->positionCollider->rect.w / 2) : (player->positionCollider->rect.w / 2));
	player->attackCollider = App->collision->AddCollider({ attack_x, player->positionCollider->rect.y - attack.y, attack.x, player->positionCollider->rect.h + attack.y * 2 }, COLLIDER_TYPE::PLAYER_HIT, false, false, std::bind(&Player::OnCollision, player, std::placeholders::_1), player);
	player->setCurrentFx("attack");
}

PlayerStateMachine *CodyIdleJumpAttackState::Update(Player *player) {
	if (player->position->z > -55) {
		player->speed.z -= player->baseSpeed * 2;
	}
	else if (player->position->z <= -55) {
		return new CodyIdleFallState();

	}
	return nullptr;
}