#include "CodyKilledState.h"
#include "Animation.h"
#include "Player.h"
#include "CodyIdleState.h"
#include "ModuleCollision.h"

CodyKilledState::CodyKilledState() : CodyStateMachine(PLAYER_STATE::PLAYER_KILLED_STATE){

}


CodyKilledState::~CodyKilledState() {
}

void CodyKilledState::Start(Player *player) {
	player->setCurrentAnimation("killed");
	player->setCurrentFx("killed");
	--player->lives;
}

PlayerStateMachine *CodyKilledState::Update(Player *player) {
	if (player->getCurrentAnimation()->Finished()) {
		if (player->lives <= 0) {
			player->positionCollider->to_delete = true;
			player->to_delete = true;
		}
		else {
			player->life = player->max_life;
			return new CodyIdleState();
		}
	}
	return nullptr;
}

PlayerStateMachine *CodyKilledState::ChangeState(PLAYER_STATE playerState) {
		return nullptr;
}