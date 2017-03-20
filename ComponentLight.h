#ifndef COMPONENTLIGHT_H
#define COMPONENTLIGHT_H

#include "Component.h"
class ComponentLight :
	public Component
{
public:
	ComponentLight(GameObject* container, std::string id);
	~ComponentLight();

	bool DrawGUI();
};

#endif // !COMPONENTLIGHT_H