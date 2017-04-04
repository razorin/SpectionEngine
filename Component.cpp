#include "Component.h"
#include "Globals.h"


Component::Component(GameObject * container, ComponentType type, std::string id) : container(container), type(type), id(id)
{
}

Component::Component(const Component * component, GameObject * container) : container(component->container), active(component->active), 
toDelete(component->toDelete), id(component->id), name(component->name), maxComponentsByGO(component->maxComponentsByGO), type(component->type)
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

const GameObject * Component::getGameObject() const
{
	return container;
}
