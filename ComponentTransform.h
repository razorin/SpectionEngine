#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

enum SpaceMode {
	SPACE_LOCAL,
	SPACE_GLOBAL
};




class ComponentTransform :
	public Component
{
public:
	ComponentTransform(GameObject* container);
	ComponentTransform(GameObject* container, Quat& rotation, float3& position, float3& scale);
	~ComponentTransform();

	//Getters
	float3 Position(SpaceMode space = SpaceMode::SPACE_LOCAL) const;
	float3 Scale(SpaceMode space = SpaceMode::SPACE_LOCAL) const;
	Quat Rotation(SpaceMode space = SpaceMode::SPACE_LOCAL) const;

	//Setters
	void SetPosition(const float3& position);
	void SetScale(const float3& scale);
	void SetRotation(const Quat& rotation);

	void CalculateTransforms();
	void CalculateLocalT();
	void CalculateGlobalT();

	//New "link node" function, called from GameObject setParent
	void SetParent(const float4x4& parentGlobalTrasnform);


private:
	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity;

	//For GUI user friendliness
	float3 rotationEuler = float3::zero;

	float4x4 localTransform = float4x4::identity;
	float4x4 globalTransform = float4x4::identity;
};

#endif // !COMPONENTTRANSFORM_H