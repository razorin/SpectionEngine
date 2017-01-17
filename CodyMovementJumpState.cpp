#include "CodyMovementJumpState.h"

#include "Player.h"
#include "Animation.h"
#include "Application.h"
#include "ModuleInput.h"

#include "CodyIdleFallState.h"
#include "CodyMovementFallState.h"
#include "CodyMovementJumpAttackState.h"


CodyMovementJumpState::CodyMovementJumpState() {
}


CodyMovementJumpState::~CodyMovementJumpState() {
}


void CodyMovementJumpState::Start(Player *player) {
	player->setCurrentAnimation("moveJump");
}

PlayerStateMachine *CodyMovementJumpState::Update(Player *player) {
	if (player->position->z > -55) {
		player->speed.z -= player->baseSpeed * 2;
		player->speed.x += (player->flipped ? -player->baseSpeed : player->baseSpeed) * 2;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
			return new CodyMovementJumpAttackState();
		}
	}
	else if (player->position->z <= -55) {
		return new CodyMovementFallState();
	}

	return nullptr;
}