#ifndef PTIMER_H
#define PTIMER_H

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

	unsigned GetTimeUS() const;
	unsigned GetTimeMS() const;
	unsigned GetTimeS() const;


private:
	static unsigned frequency;
	unsigned start = 0;
	unsigned pause = 0;

	bool isStarted = false;
	bool isPaused = false;
};

#endif