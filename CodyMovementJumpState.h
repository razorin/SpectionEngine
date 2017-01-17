#ifndef CODY_MOVEMENT_JUMP_STATE_H
#define CODY_MOVEMENT_JUMP_STATE_H

#include "CodyStateMachine.h"

class Player;

class CodyMovementJumpState : public CodyStateMachine {
public:
	CodyMovementJumpState();
	~CodyMovementJumpState();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);
};


#endif // !CODY_MOVEMENT_JUMP_STATE_H
