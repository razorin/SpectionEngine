#include "PlayerStateMachine.h"


PlayerStateMachine::PlayerStateMachine(PLAYER_STATE state) : state(state) {

}

PlayerStateMachine::~PlayerStateMachine() {

}

PLAYER_STATE PlayerStateMachine::GetState() const {
	return state;
}