#include "Component.h"
#include "Globals.h"


Component::Component(GameObject * container, ComponentType type, std::string id) : container(container), type(type), id(id)
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

bool Component::IsToDelete() const
{
	return toDelete;
}
