#ifndef CODY_IDLE_JUMP_STATE_H
#define CODY_IDLE_JUMP_STATE_H

#include "CodyStateMachine.h"
class Player;

class CodyIdleJumpState : public CodyStateMachine {
public:
	CodyIdleJumpState();
	~CodyIdleJumpState();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);
};

#endif // !CODY_IDLE_JUMP_STATE_H

