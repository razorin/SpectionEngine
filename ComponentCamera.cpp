#include "ComponentCamera.h"


ComponentCamera::ComponentCamera(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_CAMERA)
{
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::DrawGUI()
{
	return false;
}