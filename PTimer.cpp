#include "PTimer.h"
#include "SDL\include\SDL_timer.h"



Uint64 PTimer::frequency = SDL_GetPerformanceFrequency();

PTimer::PTimer()
{
}

PTimer::~PTimer()
{
}

void PTimer::Start()
{
	isStarted = true;
	start = SDL_GetPerformanceCounter();
}

void PTimer::Stop()
{
	isStarted = false;
	isPaused = false;
}

void PTimer::Pause()
{
	isPaused = true;
	pause = SDL_GetPerformanceCounter();
}

void PTimer::Resume()
{
	isPaused = false;
	start += SDL_GetPerformanceCounter() - pause;
}

void PTimer::Restart()
{
	Stop();
	Start();
}

Uint64 PTimer::GetTimeUS() const
{
	Uint64 timeUS = 0;

	if (isStarted)
	{
		if (isPaused)
		{
			timeUS = (pause - start) / frequency;
		}
		else
		{
			timeUS = (SDL_GetPerformanceCounter() - start) * 1000000 / frequency;
		}
	}

	return timeUS;
}

Uint64 PTimer::GetTimeMS() const
{
	return GetTimeUS() / 1000;
}

Uint64 PTimer::GetTimeS() const
{
	return GetTimeUS() / 1000000;
}
