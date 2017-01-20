#include "PreciseTimer.h"
#include "SDL\include\SDL.h"

float PreciseTimer::frequency = (float)SDL_GetPerformanceFrequency();

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

float PreciseTimer::Ellapsed() const {
	switch (state) {
	case TIMER_STATE::TIMER_PAUSED:
		return (pause - start) / frequency;
		break;
	default:
		return ((int)SDL_GetPerformanceCounter() - start) * 1000000 / frequency;
		break;
	}
}

float PreciseTimer::EllapsedInMilliseconds() const {
	return (Ellapsed() / (float)1000);
}

float PreciseTimer::EllapsedInSeconds() const {
	return (Ellapsed() / (float)1000000);
}