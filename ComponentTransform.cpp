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


void ComponentTransform::ResetTransforms()
{
	localTransform = float4x4::identity;
	globalTransform = float4x4::identity;
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

float3 ComponentTransform::EulerAngles(SpaceMode space) const
{
	if (space == SpaceMode::SPACE_LOCAL) {
		return eulerAngles;
	}
	else {
		//return
	}
}

float4x4 ComponentTransform::LocalTransform() const
{
	return localTransform;
}

float4x4 ComponentTransform::GlobalTransform() const
{
	return globalTransform;
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



void ComponentTransform::SetPosition(const float3 & position)
{
	this->position = position;
	CalculateLocalT();
}

void ComponentTransform::SetScale(const float3 & scale)
{
	this->scale = scale;
	CalculateLocalT();
}

void ComponentTransform::SetRotation(const Quat & rotation)
{
	CalculateLocalT();
}

void ComponentTransform::SetEulerAngles(const float3 & eulerAngles)
{
	//set euler angles. calcular target quaternion. Interpolar entre nuestra rotacion actual y el quaternion obtenido. 
	// Actualizar los valores de euler angles haciend
}

void ComponentTransform::SetTransform(const float3 & position, const float3 & scale, const Quat & rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	CalculateLocalT();
}

void ComponentTransform::SetParent(const float4x4 & parentGT)
{
	CalculateLocalT();
	parentGlobalTransform = parentGT;
	globalTransform = globalTransform * localTransform;
}

void ComponentTransform::ChangeParent(const float4x4 & newParentGT)
{
	localTransform = newParentGT.Inverted() * (parentGlobalTransform * localTransform);
	localTransform.Decompose(position, rotation, scale);
	globalTransform = newParentGT * localTransform;
}
