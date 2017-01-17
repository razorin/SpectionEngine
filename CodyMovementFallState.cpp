#include "CodyMovementFallState.h"
#include "Player.h"
#include "Animation.h"
#include "CodyIdleState.h"


CodyMovementFallState::CodyMovementFallState() {
}


CodyMovementFallState::~CodyMovementFallState() {
}

void CodyMovementFallState::Start(Player *player) {
	player->setCurrentAnimation("fall");
}

PlayerStateMachine *CodyMovementFallState::Update(Player *player) {
	PlayerStateMachine *result = nullptr;

	if (player->position->z >= 0) {
		player->animations["iddleJump"]->Reset();
		player->animations["fall"]->Reset();
		return new CodyIdleState();
	}
	else {
		if (player->getCurrentAnimation()->Finished())
			player->setCurrentAnimation("lastIddleJump");

		player->speed.z += player->baseSpeed * 2;
		player->speed.x += (player->flipped ? -player->baseSpeed : player->baseSpeed) * 2;

	}

	return result;
}