#ifndef PRECISETIMER_H
#define PRECISETIMER_H

#include "Timer.h"

class PreciseTimer {
public:
	PreciseTimer();
	virtual ~PreciseTimer();

	void Start();
	void Stop();
	void Restart();
	void Pause();
	void Resume();
	float Ellapsed() const;
	float EllapsedInMilliseconds() const;
	float EllapsedInSeconds() const;

private:
	float start = 0;
	float pause = 0;
	static float frequency;

public:
	TIMER_STATE state = TIMER_STOPPED;
};

#endif // PRECISETIMER_H

