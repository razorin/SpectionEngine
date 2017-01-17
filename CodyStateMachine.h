#ifndef CODY_STATE_MACHINE_H
#define CODY_STATE_MACHINE_H

#include "PlayerStateMachine.h"


class Player;

class CodyStateMachine : public PlayerStateMachine {
public:
	CodyStateMachine(PLAYER_STATE state = PLAYER_STATE::PLAYER_OTHER_STATE);
	~CodyStateMachine();

	void Start(Player *player);
	PlayerStateMachine * Update(Player *player);
	PlayerStateMachine *ChangeState(PLAYER_STATE playerState);

};

#endif // !CODY_STATE_MACHINE_H

