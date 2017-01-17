#include "CodyMoveState.h"
#include "Player.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Animation.h"
#include "Point.h"
#include "CodyIdleState.h"
#include "CodyAttackStateOne.h"
#include "CodyMovementJumpState.h"


CodyMoveState::CodyMoveState() {
}


CodyMoveState::~CodyMoveState() {
}

void CodyMoveState::Start(Player *player) {
	player->setCurrentAnimation("movement");
}

PlayerStateMachine *CodyMoveState::Update(Player *player) {
	PlayerStateMachine *result = nullptr;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		player->speed.x -= player->baseSpeed;
		player->flipped = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		player->speed.x += player->baseSpeed;
		player->flipped = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		player->speed.y += player->baseSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		player->speed.y -= player->baseSpeed;
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		return new CodyAttackStateOne();
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		return new CodyMovementJumpState();
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_E) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
		return new CodyIdleState();
	}

	return result;
}
