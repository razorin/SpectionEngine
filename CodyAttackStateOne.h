#ifndef CODY_ATTACK_STATE_ONE_H
#define CODY_ATTACK_STATE_ONE_H

#include "CodyStateMachine.h"

class Player;

class CodyAttackStateOne :	public CodyStateMachine {
public:
	CodyAttackStateOne();
	~CodyAttackStateOne();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);
};

#endif // !CODY_ATTACK_STATE_ONE_H

