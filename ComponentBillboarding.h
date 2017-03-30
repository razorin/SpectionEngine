#include "Component.h"
#include "assimp\cimport.h"
#include "MathGeoLib\include\MathGeoLib.h"


class GameObject;


class ComponentBillboarding : public Component
{
public:
	ComponentBillboarding(GameObject* container, std::string id, float2 size, aiString texturePath);
	~ComponentBillboarding();

	void ComputeQuad(float3 cameraPosition);
	bool DrawGUI();

private:
	unsigned texture = 0;
	float2 size = { 0, 0 };
	aiString texturePath;

};

