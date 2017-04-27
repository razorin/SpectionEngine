#include "ComponentAnim.h"
#include "Application.h"
#include "ModuleAnimation.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include <map>

ComponentAnim::ComponentAnim(GameObject * container, std::string id) : Component(container, ComponentType::COMPONENT_TYPE_ANIMATION, id), isPlaying(false)
{
}

ComponentAnim::ComponentAnim(GameObject * container, const ComponentAnim * animation) : Component(container, ComponentType::COMPONENT_TYPE_ANIMATION, animation->id), isPlaying(animation->isPlaying),
numClips(animation->numClips), instanceId(animation->instanceId)
{
	for (auto it = animation->clips.begin(); it != animation->clips.end(); ++it) {
		AddClip((*it).second);
	}
}

ComponentAnim::~ComponentAnim()
{
}


bool ComponentAnim::AddClip(Anim * clip)
{
	bool ret = false;

	if (!(ret = CheckClipInList(clip)))
	{
		//clips.push_back(clip);
		clips[clip->name.data] =  clip;
		numClips++;
		ret = true;
	}

	return ret;
}

bool ComponentAnim::SetCurrentClip(Anim * clip)
{
	bool ret = false;

	if (!(ret = CheckClipInList(clip)))
		AddClip(clip);

	currentClip = clip;

	return ret;
}

bool ComponentAnim::CheckClipInList(Anim * clip)
{
	return CheckClipInList(clip->name.data);
}

bool ComponentAnim::CheckClipInList(const char * animName)
{
	std::map<std::string, Anim*>::iterator it = clips.find(animName);
	return it != clips.end();
}

void ComponentAnim::Play(bool loop)
{
	instanceId = App->animator->Play(currentClip->name.data);
	isPlaying = true;
}

void ComponentAnim::Play(bool loop, const char * animName)
{
	if (CheckClipInList(animName))
	{
		instanceId = App->animator->Play(animName);
		isPlaying = true;
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

