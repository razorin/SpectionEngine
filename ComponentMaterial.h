#ifndef COMPONENTMATERIAL_H
#define COMPONENTMATERIAL_H

#include "Component.h"
class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial(GameObject* container, std::string id);
	ComponentMaterial(GameObject* container, ComponentMaterial* mat);
	~ComponentMaterial();

	bool DrawGUI();
};

#endif // !COMPONENTMATERIAL_H