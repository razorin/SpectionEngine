#ifndef CODY_MOVEMENT_JUMP_ATTACK_STATE_H
#define CODY_MOVEMENT_JUMP_ATTACK_STATE_H

#include "CodyStateMachine.h"

class Player;

class CodyMovementJumpAttackState :
	public CodyStateMachine {
public:
	CodyMovementJumpAttackState();
	~CodyMovementJumpAttackState();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);
};
#endif // !CODY_MOVEMENT_JUMP_ATTACK_STATE_H

