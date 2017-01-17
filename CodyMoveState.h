#ifndef CODY_MOVE_STATE_H
#define CODY_MOVE_STATE_H

#include "CodyStateMachine.h"

class CodyMoveState : public CodyStateMachine {
public:
	CodyMoveState();
	~CodyMoveState();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);
};

#endif // !CODY_MOVE_STATE_H

