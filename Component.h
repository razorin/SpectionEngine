#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

enum ComponentType {
	COMPONENT_TYPE_CAMERA,
	COMPONENT_TYPE_SCRIPT,
	COMPONENT_TYPE_LIGHT,
	COMPONENT_TYPE_TRANSFORM,
	COMPONENT_TYPE_MATERIAL,
	COMPONENT_TYPE_MODEL
};

class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

private:
	GameObject *parent = nullptr;
	bool enable = true;
	std::string name = "";

public:
	ComponentType type;
	int maxNumberOfComponentByGameObject = 1;
};

#endif // !COMPONENT_H

