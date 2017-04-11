#ifndef _COMPONENTANIM_H
#define _COMPONENTANIM_H

#include "Component.h"

class ComponentAnim : public Component
{
public:
	ComponentAnim(GameObject* container, std::string name);
	~ComponentAnim();

	void Play(const char* animName, bool loop = true);
	void Stop();
	void EndClip();

	bool DrawGUI();

public:
	unsigned instanceId = -1;
	bool isPlaying = false;
	unsigned blendTime = 0;
};

#endif