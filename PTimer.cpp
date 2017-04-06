#include "PTimer.h"
#include "SDL\include\SDL.h"


unsigned PTimer::frequency = SDL_GetPerformanceFrequency();

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

unsigned PTimer::GetTimeUS() const
{
	unsigned timeUS = 0;

	if (isStarted)
	{
		if (isPaused)
		{
			timeUS = (pause - start) / frequency;
		}
		else
		{
			timeUS = (SDL_GetPerformanceCounter() - start) / frequency;
		}
	}

	return timeUS;
}

unsigned PTimer::GetTimeMS() const
{

	return GetTimeUS() / 1000;
}

unsigned PTimer::GetTimeS() const
{
	return GetTimeS() / 1000000;
}
