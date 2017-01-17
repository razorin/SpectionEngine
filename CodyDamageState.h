#ifndef CODY_DAMAGE_STATE_H
#define CODY_DAMAGE_STATE_H

#include "CodyStateMachine.h"

class Player;

class CodyDamageState : public CodyStateMachine {
public:
	CodyDamageState();
	~CodyDamageState();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);
};

#endif // !CODY_DAMAGE_STATE_H
