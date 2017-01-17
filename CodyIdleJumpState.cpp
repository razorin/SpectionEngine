#include "CodyIdleJumpState.h"
#include "Player.h"
#include "Point.h"
#include "Application.h"
#include "ModuleInput.h"
#include "CodyIdleFallState.h"
#include "CodyIdleJumpAttackState.h"

CodyIdleJumpState::CodyIdleJumpState() {
}


CodyIdleJumpState::~CodyIdleJumpState() {
}

void CodyIdleJumpState::Start(Player *player) {
	player->setCurrentAnimation("iddleJump");
}

PlayerStateMachine *CodyIdleJumpState::Update(Player *player) {
	if (player->position->z > -55) {
		player->speed.z -= player->baseSpeed * 2;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
			return new CodyIdleJumpAttackState();
		}
	}
	else if (player->position->z <= -55) {
		return new CodyIdleFallState();
	}

	return nullptr;
}
