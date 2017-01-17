#ifndef CODY_IDLE_FALL_STATE_H
#define CODY_IDLE_FALL_STATE_H

#include "CodyStateMachine.h"

class Player;

class CodyIdleFallState : public CodyStateMachine {
public:
	CodyIdleFallState();
	~CodyIdleFallState();
	void Start(Player *player);
	PlayerStateMachine * Update(Player *player);
};

#endif // !CODY_IDLE_FALL_STATE_H

