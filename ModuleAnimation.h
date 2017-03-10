#ifndef __MODULEANIMATION_H__
#define __MODULEANIMATION_H__

#include "Module.h"
#include <map>
#include "MathGeoLib\include\MathGeoLib.h"

struct AnimationChannel {
	std::string nodeName;
	int numFrames;
	float3* positionKeyFrames;
	float3* rotationKeyFrames;
	float3* scalingKeyFrames;
};

struct Animation {
	std::string name;
	double duration;
	int numChannels;
	AnimationChannel* channels;
};

class ModuleAnimation :
	public Module
{
public:
	ModuleAnimation(const JSON_Object *json, bool active);
	~ModuleAnimation();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Load(const char* path, const char* file);

public:
	std::map<std::string, Animation*> animations;

};

#endif // !__MODULEANIMATION_H__