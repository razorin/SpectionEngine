#ifndef CODY_IDLE_STATE
#define CODY_IDLE_STATE

#include "CodyStateMachine.h"

class Player;

class CodyIdleState : public CodyStateMachine {
public:
	CodyIdleState();
	~CodyIdleState();

	void Start(Player *player);
	PlayerStateMachine * Update(Player *player);
};

#endif // !CODY_IDLE_STATE

