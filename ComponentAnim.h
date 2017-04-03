#ifndef _COMPONENTANIM_H
#define _COMPONENTANIM_H

#include "Globals.h"
#include "Component.h"
#include "ModuleAnimation.h"
#include "AssimpIncludes.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <vector>

class ComponentAnim : public Component
{
public:
	ComponentAnim(GameObject* container, std::string name);
	ComponentAnim(const ComponentAnim *animation, GameObject * container);
	~ComponentAnim();

	bool AddClip(Anim* clip);
	bool SetCurrentClip (Anim* clip);
	bool CheckClipInList(Anim* clip);
	bool CheckClipInList(const char* animName);

	void Play(bool loop = true);
	void Play(bool loop, const char* animName);
	void Stop();
	void EndClip();

	bool DrawGUI();

public:
	Anim* currentClip = nullptr;
	bool isPlaying = false;
	unsigned numClips = 0;
	std::vector<Anim*> clips;
	uint instanceId = -1;
};

#endif