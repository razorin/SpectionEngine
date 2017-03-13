#include "ComponentMaterial.h"


ComponentMaterial::ComponentMaterial(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_MATERIAL)
{
}

ComponentMaterial::~ComponentMaterial()
{
}

bool ComponentMaterial::DrawGUI()
{
	return false;
}