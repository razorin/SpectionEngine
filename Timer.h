#ifndef TIMER_H
#define TIMER_H

enum TIMER_STATE {
	TIMER_STARTED,
	TIMER_STOPPED,
	TIMER_PAUSED
};

class Timer {
public:
	Timer();
	virtual ~Timer();

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

#endif // TIMER_H

