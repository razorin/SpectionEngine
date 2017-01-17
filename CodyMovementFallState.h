
#ifndef CODY_MOVEMENT_FALL_STATE_H
#define CODY_MOVEMENT_FALL_STATE_H

#include "CodyStateMachine.h"

class Player;

class CodyMovementFallState : public CodyStateMachine {
public:
	CodyMovementFallState();
	~CodyMovementFallState();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);
};

#endif // !CODY_MOVEMENT_FALL_STATE_H

