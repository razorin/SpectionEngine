#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "GameObject.h"

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

	void ResetTransforms();

	//Getters
	float3 Position(SpaceMode space = SpaceMode::SPACE_LOCAL) const;
	float3 Scale(SpaceMode space = SpaceMode::SPACE_LOCAL) const;
	Quat Rotation(SpaceMode space = SpaceMode::SPACE_LOCAL) const;
	float3 EulerAngles(SpaceMode space = SpaceMode::SPACE_LOCAL) const;
	float4x4 LocalTransform() const;
	float4x4 GlobalTransform() const;

	void CalculateTransforms();
	void CalculateLocalT();
	void CalculateGlobalT();

	void SetPosition(const float3& position);
	void SetScale(const float3& scale);
	void SetRotation(const Quat& rotation);
	void SetEulerAngles(const float3& eulerAngles);
	void Rotate(float angles, float3 axis);
	void SetTransform(const float3& position, const float3& scale, const Quat& rotation);
	void SetParent(const float4x4& parentGT);
	void ChangeParent(const float4x4& newParentGT);

	bool DrawGUI();


private:
	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity;
	float3 eulerAngles = float3::zero;

	float4x4 localTransform = float4x4::identity;
	float4x4 globalTransform = float4x4::identity;
	float4x4 parentGlobalTransform = float4x4::identity;
};

#endif // !COMPONENTTRANSFORM_H