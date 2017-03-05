#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform :
	public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

public:
	float3 position;
	float3 rotation;
	float3 scale;
};

#endif // !COMPONENTTRANSFORM_H