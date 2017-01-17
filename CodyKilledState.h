#ifndef CODY_KILLED_STATE_H
#define CODY_KILLED_STATE_H


#include "CodyStateMachine.h"

class Player;

class CodyKilledState : public CodyStateMachine {
public:
	CodyKilledState();
	~CodyKilledState();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);
	PlayerStateMachine *ChangeState(PLAYER_STATE playerState);
};

#endif // !CODY_KILLED_STATE_H

