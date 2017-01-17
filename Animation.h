#pragma once
#include <vector>
#include "SDL\include\SDL.h"
#include "Globals.h"
#include "Point.h"

using namespace std;

struct Frame {
	SDL_Rect section;
	int offset_x;
	int offset_y;
	Frame(const SDL_Rect &section, int offset_x = 0, int offset_y = 0) : section(section), offset_x(offset_x), offset_y(offset_y){

	}
};

struct Icon {
	SDL_Rect section;
	iPoint position;

	Icon(const SDL_Rect &section, const iPoint &position) : section(section), position(position) {

	}

	Icon(const Icon &other) : section(other.section), position(other.position) {

	}
};


class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	vector<Frame> frames;

private:
	float current_frame = 0.0f;
	int loops = 0;

public:
	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), frames(anim.frames)
	{}

	Frame& GetCurrentFrame()
	{
		float last_frame = (float) frames.size();

		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : MAX(last_frame - 1.0f, 0.0f);
			loops++;
		} 

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
		loops = 0;
	}
};