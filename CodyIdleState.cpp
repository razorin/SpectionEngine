#include "CodyIdleState.h"
#include "CodyIdleJumpState.h"
#include "CodyMoveState.h"
#include "Player.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Animation.h"
#include "CodyAttackStateOne.h"


CodyIdleState::CodyIdleState() {
}


CodyIdleState::~CodyIdleState() {
}

void CodyIdleState::Start(Player *player) {
	player->setCurrentAnimation("idle");
}

PlayerStateMachine * CodyIdleState::Update(Player *player) {
	PlayerStateMachine *result = nullptr;
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		result = new CodyIdleJumpState();
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

		result = new CodyMoveState();
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		return new CodyAttackStateOne();
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		result = nullptr;
	}

	return result;
}