#include "Timer.h"
#include "SDL\include\SDL.h"


Timer::Timer() {
}


Timer::~Timer() {
}


void Timer::Start() {
	start = (int)SDL_GetTicks();
	state = TIMER_STATE::TIMER_STARTED;
}

void Timer::Stop() {
	start = 0;
	state = TIMER_STATE::TIMER_STOPPED;
}

void Timer::Pause() {
	pause = (int)SDL_GetTicks();
	state = TIMER_STATE::TIMER_PAUSED;
}

void Timer::Resume() {
	start += ((int)SDL_GetTicks() ) - pause;
	state = TIMER_STATE::TIMER_STARTED;
}

int Timer::Ellapsed() const {
	switch (state) {
	case TIMER_STATE::TIMER_PAUSED:
		return pause - start;
		break;
	default:
		return ((int)SDL_GetTicks()) - start;
		break;
	}
	
}

float Timer::EllapsedInSeconds() const{
	return ((float) Ellapsed() / (float)1000);
}