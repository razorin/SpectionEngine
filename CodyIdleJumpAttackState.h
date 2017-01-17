#ifndef CODY_IDLE_JUMP_ATTACK_STATE_H
#define CODY_IDLE_JUMP_ATTACK_STATE_H

#include "CodyStateMachine.h"
#include "Player.h"

class CodyIdleJumpAttackState : public CodyStateMachine {
public:
	CodyIdleJumpAttackState();
	~CodyIdleJumpAttackState();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);
};

#endif // !CODY_IDLE_JUMP_ATTACK_STATE_H#

