#ifndef COMPONENTMATERIAL_H
#define COMPONENTMATERIAL_H

#include "Component.h"
class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial(GameObject* container);
	~ComponentMaterial();

	bool DrawGUI();
};

#endif // !COMPONENTMATERIAL_H