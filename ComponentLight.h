#ifndef COMPONENTLIGHT_H
#define COMPONENTLIGHT_H

#include "Component.h"
class ComponentLight :
	public Component
{
public:
	ComponentLight(GameObject* container);
	~ComponentLight();
};

#endif // !COMPONENTLIGHT_H