#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include "Component.h"
class ComponentModel :
	public Component
{
public:
	ComponentModel(GameObject* container);
	~ComponentModel();

	bool DrawGUI();
};

#endif // !COMPONENTMODEL_H