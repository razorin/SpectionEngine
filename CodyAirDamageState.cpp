#include "CodyAirDamageState.h"
#include "Player.h"
#include "Animation.h"
#include "CodyIdleState.h"
#include "CodyKilledState.h"


CodyAirDamageState::CodyAirDamageState() :  CodyStateMachine(PLAYER_STATE::PLAYER_AIR_DAMAGE_STATE){
}


CodyAirDamageState::~CodyAirDamageState() {
}

void CodyAirDamageState::Start(Player *player) {
	player->setCurrentAnimation("airDamage");
}

PlayerStateMachine *CodyAirDamageState::Update(Player *player) {
	if (player->position->z >= 0) {
		player->animations["airDamage"]->Reset();
		if (player->life > 0)
			return new CodyIdleState();
		return new CodyKilledState();
	}
	else {
		if (player->getCurrentAnimation()->Finished())
			player->setCurrentAnimation("airDamage");
		player->speed.z += player->baseSpeed * 2;
		player->speed.x += 4 * (direction > 0 ? player->baseSpeed : (direction < 0 ? -player->baseSpeed : 0));
	}

	return nullptr;
}