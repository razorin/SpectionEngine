#include "ComponentAnim.h"
#include "Application.h"
#include "ModuleAnimation.h"
#include "ComponentTransform.h"
#include "GameObject.h"


ComponentAnim::ComponentAnim(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_ANIMATION), isPlaying(false)
{
}

ComponentAnim::~ComponentAnim()
{
}

bool ComponentAnim::AddClip(Anim * clip)
{
	bool ret = false;

	if (!(ret = CheckClipInList(clip)))
	{
		clips.push_back(clip);
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
	bool found = false;
	for (int i = 0; (i < clips.size() && !found); i++)
	{
		if (clip->name == clips[i]->name)
			found = true;
	}
	return found;
}

bool ComponentAnim::CheckClipInList(const char * animName)
{
	bool found = false;
	for (int i = 0; (i < clips.size() && !found); i++)
	{
		if (animName == clips[i]->name.data)
			found = true;
	}
	return found;
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

