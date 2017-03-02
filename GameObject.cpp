#include "GameObject.h"

#include "Component.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
	for (std::list<GameObject *>::iterator it = childrens.begin(); it != childrens.end();) {
		delete (*it);
		it = childrens.erase(it);
		++it;
	}

	for (std::list<Component *>::iterator it = components.begin(); it != components.end();) {
		delete (*it);
		it = components.erase(it);
		++it;
	}
}

Component * GameObject::addComponent(const ComponentType &type)
{
	Component *result = nullptr;
	//TODO: Create component
	//TODO: Review max number of components allowed by GameObject
	//TODO: Increment componentCounter!!!
	//switch (type) {
	//case ComponentType::COMPONENT_TYPE_CAMERA:

	//}

	return result;
}

void GameObject::removeComponent(Component *)
{
	//Find Component
	//Remove and release element
	//Decrement componentCounter
}

void GameObject::Update()
{
}

