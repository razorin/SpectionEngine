#ifndef COMPONENT_BILLBOARDING_H
#define COMPONENT_BILLBOARDING_H

#include "Component.h"
#include "assimp\cimport.h"
#include "MathGeoLib\include\MathGeoLib.h"


class GameObject;

struct Billboard {
	Billboard() {};
	Billboard(float2 size) : size(size) {}
	float2 size = { 0, 0 };
};

class ComponentBillboarding : public Component
{
public:
	ComponentBillboarding(GameObject* container, std::string id, float2 size);
	~ComponentBillboarding();

	std::vector<float3> ComputeQuad(const float3 cameraPosition);
	bool DrawGUI();

private:
	Billboard billboard;
};

#endif // !COMPONENT_BILLBOARDING_H