#include "ComponentAnim.h"
#include "Application.h"
#include "ModuleAnimation.h"
#include "ComponentTransform.h"
#include "GameObject.h"


ComponentAnim::ComponentAnim(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_ANIMATION)
{
}

ComponentAnim::~ComponentAnim()
{
}

void ComponentAnim::AddClip(Anim * clip)
{
}

void ComponentAnim::SetCurrentClip(Anim * clip)
{
}

void ComponentAnim::Play(bool loop, const char * animName)
{
}

void ComponentAnim::Stop()
{
}

