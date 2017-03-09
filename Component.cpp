#include "Component.h"
#include "Globals.h"


Component::Component(GameObject * container, ComponentType type) : container(container), type(type)
{
}

Component::~Component()
{
}

void Component::SetActive(bool active)
{
	this->active = active;
}

bool Component::IsActive()
{
	return active;
}