#include "CodyDamageState.h"
#include "Player.h"
#include "Animation.h"
#include "CodyIdleState.h"
#include "CodyKilledState.h"

CodyDamageState::CodyDamageState() : CodyStateMachine(PLAYER_STATE::PLAYER_DAMAGE_STATE) {
}


CodyDamageState::~CodyDamageState() {
}

void CodyDamageState::Start(Player *player) {
	player->setCurrentAnimation("damage");
	player->setCurrentFx("damage");
}

PlayerStateMachine * CodyDamageState::Update(Player *player) {
	if (player->getCurrentAnimation()->Finished()) {
		if(player->life > 0)
			return new CodyIdleState();
		return new CodyKilledState();
	}
	return nullptr;
}
