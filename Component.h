#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "GameObject.h"
#include "MemLeaks.h"

enum ComponentType {
	COMPONENT_TYPE_ANIMATION,
	COMPONENT_TYPE_CAMERA,
	COMPONENT_TYPE_LIGHT,
	COMPONENT_TYPE_MATERIAL,
	COMPONENT_TYPE_MESH,
	COMPONENT_TYPE_SCRIPT,
	COMPONENT_TYPE_TRANSFORM,
	COMPONENT_TYPE_UNDEFINED
};

class GameObject;

class Component
{
public:
	Component(){}
	Component(GameObject* container, ComponentType type, std::string id);
	Component(const Component *component, GameObject * container);
	virtual ~Component();

	//We just do active for go and components. Unity uses enable = .. for components
	void SetActive(bool active);
	bool IsActive();
	bool IsToDelete() const;

	virtual bool DrawGUI() = 0;

public:
	ComponentType type = ComponentType::COMPONENT_TYPE_UNDEFINED;

protected:
	GameObject *container = nullptr;
	bool active = true;
	bool toDelete = false;
	std::string id = "";
	std::string name;

public:
	int maxComponentsByGO = 1;
};

#endif // !COMPONENT_H

