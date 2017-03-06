#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "GameObject.h"

enum ComponentType {
	COMPONENT_TYPE_CAMERA,
	COMPONENT_TYPE_SCRIPT,
	COMPONENT_TYPE_LIGHT,
	COMPONENT_TYPE_TRANSFORM,
	COMPONENT_TYPE_MATERIAL,
	COMPONENT_TYPE_MODEL,
	COMPONENT_TYPE_UNDEFINED
};

class GameObject;

class Component
{
public:
	Component(GameObject* container, ComponentType type);
	virtual ~Component();

	//We just do active for go and components. Unity uses enable = .. for components
	void SetActive(bool active);
	bool IsActive();

public:
	ComponentType type = ComponentType::COMPONENT_TYPE_UNDEFINED;

protected:
	GameObject *container = nullptr;
	bool active = true;

public:
	int maxComponentsByGO = 1;
};

#endif // !COMPONENT_H

