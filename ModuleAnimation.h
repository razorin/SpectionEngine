#ifndef __MODULEANIMATION_H__
#define __MODULEANIMATION_H__

#include "Globals.h"
#include "Module.h"
#include <map>
#include <vector>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp\anim.h"


struct AnimChannel
{
	aiString name;
	aiVector3D* positions = nullptr;
	aiQuaternion* rotations = nullptr;
	aiVector3D* scales = nullptr;
	uint numPositions = 0;
	uint numRotations = 0;
	uint numScales = 0;
	uint numKeyframes = 0;
};

struct Anim
{
	aiString name;
	uint duration = 0;
	uint numChannels = 0;
	AnimChannel* channels = nullptr;
};

struct AnimInstance
{
	Anim* animation;
	uint id = 0;
	uint time = 0;
	bool loop = true;
	AnimInstance* next = nullptr;
	uint blendDuration = 0;
	uint blendTime = 0;
};




class ModuleAnimation :
	public Module
{
public:
	ModuleAnimation();
	~ModuleAnimation();

	bool Init();
	bool Start();
	void Load(const char* path, const char* file);
	bool CleanUp();
	update_status Update(float dt = 0);

	uint Play(const char* goName);
	void Stop(uint instanceId);
	void BlendTo(uint instanceId, const char* name, uint blendTime);
	bool GetTransform(uint instanceId, const char* channel, aiVector3D& position, aiQuaternion& rotation);

public:
	std::map<std::string, Anim*> animations;
	std::vector<AnimInstance*> instances;
	std::vector<uint*> holes;




};

#endif