#ifndef _COMPONENTANIM_H
#define _COMPONENTANIM_H

#include "Component.h"
#include "ModuleAnimation.h"
#include "AssimpIncludes.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <vector>

class ComponentAnim : public Component
{
public:
	ComponentAnim(GameObject* container);
	~ComponentAnim();

	void AddClip(Anim* clip);
	void SetCurrentClip (Anim* clip);
	void Play(bool loop, const char* animName);
	void Stop();

public:
	Anim* currentCLip = nullptr;

	unsigned numClips = 0;
	std::vector<Anim*> clips;

	uint playingClipInstanceId = -1;
};

#endif