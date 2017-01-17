#pragma once
#include "CodyStateMachine.h"
class CodyAirDamageState : public CodyStateMachine {
public:
	CodyAirDamageState();
	~CodyAirDamageState();
	void Start(Player *player);
	PlayerStateMachine *Update(Player *player);


};

