#include "PreciseTimer.h"
#include "SDL\include\SDL.h"


PreciseTimer::PreciseTimer() {
}


PreciseTimer::~PreciseTimer() {
}


void PreciseTimer::Start() {
	start = (int)SDL_GetPerformanceCounter();
	state = TIMER_STATE::TIMER_STARTED;
}

void PreciseTimer::Stop() {
	start = 0;
	state = TIMER_STATE::TIMER_STOPPED;
}

void PreciseTimer::Pause() {
	pause = (int)SDL_GetPerformanceCounter();
	state = TIMER_STATE::TIMER_PAUSED;
}

void PreciseTimer::Resume() {
	start += ((int)SDL_GetPerformanceCounter()) - pause;
	state = TIMER_STATE::TIMER_STARTED;
}

int PreciseTimer::Ellapsed() const {
	switch (state) {
	case TIMER_STATE::TIMER_PAUSED:
		return pause - start;
		break;
	default:
		return (((int)SDL_GetPerformanceCounter()) - start) / SDL_GetPerformanceFrequency();
		break;
	}

}

float PreciseTimer::EllapsedInSeconds() const {
	return ((float)Ellapsed() / (float)1000);
}