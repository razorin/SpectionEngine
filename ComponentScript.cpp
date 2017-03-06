#include "ComponentScript.h"


ComponentScript::ComponentScript(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_SCRIPT)
{
}

ComponentScript::~ComponentScript()
{
}
