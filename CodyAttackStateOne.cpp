#include "CodyAttackStateOne.h"
#include "Player.h"
#include "Animation.h"
#include "CodyIdleState.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCollision.h"

CodyAttackStateOne::CodyAttackStateOne() {
}


CodyAttackStateOne::~CodyAttackStateOne() {
}

void CodyAttackStateOne::Start(Player *player) {


	Attack attack;

	switch (player->hits) {
	case 2:
		attack = player->attacks.at(player->hits);
		break;
	case 3:
		attack = player->attacks.at(player->hits);
		break;
	default:
		attack = player->attacks.at(0);
		break;
	}

	player->setCurrentAnimation(attack.name);


	if (player->attackCollider != nullptr) {
		player->attackCollider->to_delete = true;
	}
	int attack_x = player->positionCollider->rect.x + (player->flipped ? -(player->positionCollider->rect.w / 2) : (player->positionCollider->rect.w / 2));
	player->attackCollider = App->collision->AddCollider({attack_x, player->positionCollider->rect.y - attack.y, attack.x, player->positionCollider->rect.h + attack.y * 2}, COLLIDER_TYPE::PLAYER_HIT, false, false, std::bind(&Player::OnCollision, player, std::placeholders::_1), player);
	player->setCurrentFx("attack");
}

PlayerStateMachine *CodyAttackStateOne::Update(Player *player) {
	if (player->getCurrentAnimation()->Finished()) {
		return new CodyIdleState();
	}
	return nullptr;
}