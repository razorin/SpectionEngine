#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include "Component.h"
class ComponentModel :
	public Component
{
public:
	ComponentModel(GameObject* container, std::string id);
	ComponentModel(GameObject* container, ComponentModel *component) : Component(component, container){}
	~ComponentModel();

	bool DrawGUI();
};

#endif // !COMPONENTMODEL_H