#include "ModuleAnimation.h"



ModuleAnimation::ModuleAnimation() : Module(true)
{
}


ModuleAnimation::~ModuleAnimation()
{
}

bool ModuleAnimation::Init()
{
	return true;
}

bool ModuleAnimation::Start()
{
	return true;
}

void ModuleAnimation::Load(const char * name, const char * file)
{
}

bool ModuleAnimation::CleanUp()
{
	return true;
}

update_status ModuleAnimation::Update(float dt)
{
	return UPDATE_CONTINUE;
}

uint ModuleAnimation::Play(const char * goName)
{
	return uint();
}

void ModuleAnimation::Stop(uint instanceId)
{
}

void ModuleAnimation::BlendTo(uint instanceId, const char * name, uint blendTime)
{
}

bool ModuleAnimation::GetTransform(uint instanceId, const char * channel, aiVector3D & position, aiQuaternion & rotation)
{
	return false;
}



