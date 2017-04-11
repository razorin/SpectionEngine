#include "ComponentAnim.h"
#include "Application.h"
#include "ModuleAnimation.h"
#include "GameObject.h"

ComponentAnim::ComponentAnim(GameObject * container, std::string id) :
	Component(container, ComponentType::COMPONENT_TYPE_ANIMATION, id), isPlaying(false), blendTime(250.0f)
{
}

ComponentAnim::~ComponentAnim()
{
}

void ComponentAnim::Play(const char * animName, bool loop)
{
	if (!isPlaying)
	{
		instanceId = App->animator->Play(animName);
		isPlaying = true;
	}
	else
	{
		App->animator->BlendTo(instanceId, animName, blendTime);
	}
}

void ComponentAnim::Stop()
{
	App->animator->Stop(instanceId);
	isPlaying = false;
	instanceId = -1;
}

void ComponentAnim::EndClip()
{
	isPlaying = false;
	instanceId = -1;
}

bool ComponentAnim::DrawGUI()
{
	return false;
}

