#include "ComponentTransform.h"
#include <assert.h>




ComponentTransform::ComponentTransform(GameObject* container) : Component(container, ComponentType::COMPONENT_TYPE_TRANSFORM)
{
}

ComponentTransform::ComponentTransform(GameObject* container, Quat& rotation, float3& position, float3& scale) :
	Component(container, ComponentType::COMPONENT_TYPE_TRANSFORM), rotation(rotation), position(position), scale(scale)
{
}

ComponentTransform::~ComponentTransform()
{
}

float3 ComponentTransform::Position(SpaceMode space) const
{
	if (space == SpaceMode::SPACE_LOCAL) {
		return position;
	}
	else {
		return globalTransform.TranslatePart();
	}
}

float3 ComponentTransform::Scale(SpaceMode space) const
{
	if (space == SpaceMode::SPACE_LOCAL) {
		return scale;
	}
	else {
		//return
	}
}

Quat ComponentTransform::Rotation(SpaceMode space) const
{
	if (space == SpaceMode::SPACE_LOCAL) {
		return rotation;
	}
	else {
		//return
	}
}

float4x4 ComponentTransform::GlobalTransform() const
{
	return globalTransform;
}


void ComponentTransform::SetPosition(const float3 & position)
{
	this->position = position;
}

void ComponentTransform::SetScale(const float3 & scale)
{
	this->scale = scale;
}

void ComponentTransform::SetRotation(const Quat & rotation)
{
	this->rotation = rotation;
}


void ComponentTransform::CalculateTransforms()
{
	CalculateLocalT();
	CalculateGlobalT();
}

void ComponentTransform::CalculateLocalT()
{
	localTransform = float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::CalculateGlobalT()
{
	GameObject* parentGO = container->GetParent();
	if (parentGO != nullptr)
	{
		globalTransform = parentGO->transform->globalTransform * localTransform;
	}
	else
	{
		globalTransform = localTransform;
	}
}

void ComponentTransform::SetParent(const float4x4 & newParentGT)
{
	//We change the parent but we want to keep global pos, rot and scale

	GameObject* parentGO = container->GetParent();
	if (parentGO != nullptr)
	{
		localTransform = newParentGT.Inverted() * (parentGO->transform->globalTransform * localTransform);
	}
	else {
		localTransform = newParentGT.Inverted() * localTransform;
	}
	localTransform.Decompose(position, rotation, scale);
	globalTransform = newParentGT * localTransform;
}
