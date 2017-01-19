#ifndef PRECISETIMER_H
#define PRECISETIMER_H

enum TIMER_STATE {
	TIMER_STARTED,
	TIMER_STOPPED,
	TIMER_PAUSED
};

class PreciseTimer {
public:
	PreciseTimer();
	virtual ~PreciseTimer();

	void Start();
	void Stop();
	void Pause();
	void Resume();
	int Ellapsed() const;
	float EllapsedInSeconds() const;

private:
	int start = 0;
	int pause = 0;

public:
	TIMER_STATE state = TIMER_STOPPED;
};

#endif // PRECISETIMER_H

