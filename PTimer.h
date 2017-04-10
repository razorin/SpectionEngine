#ifndef PTIMER_H
#define PTIMER_H

typedef unsigned __int64 Uint64;

class PTimer
{
public:
	PTimer();
	~PTimer();

	void Start();
	void Stop();
	void Pause();
	void Resume();
	void Restart();

	Uint64 GetTimeUS() const;
	Uint64 GetTimeMS() const;
	Uint64 GetTimeS() const;


private:
	static Uint64 frequency;
	Uint64 start = 0;
	Uint64 pause = 0;

	bool isStarted = false;
	bool isPaused = false;
};

#endif