#include "ComponentLight.h"



ComponentLight::ComponentLight(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_LIGHT)
{
}

ComponentLight::~ComponentLight()
{
}

bool ComponentLight::DrawGUI()
{
	return false;
}