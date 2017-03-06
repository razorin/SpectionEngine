#include "ComponentModel.h"


ComponentModel::ComponentModel(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_MODEL)
{
}

ComponentModel::~ComponentModel()
{
}
