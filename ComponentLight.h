#ifndef COMPONENTLIGHT_H
#define COMPONENTLIGHT_H

#include "Component.h"

class Light;

class ComponentLight :
	public Component
{
public:
	ComponentLight(GameObject* container, std::string id, Light* light);
	ComponentLight(GameObject* container, ComponentLight* component);
	~ComponentLight();

	bool DrawGUI();

private:
	Light* light = nullptr;
};

#endif // !COMPONENTLIGHT_H