#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "assimp\cimport.h"
#include "MathGeoLib\include\MathGeoLib.h"


class GameObject;

class Billboard {
public:
	Billboard();
	Billboard(float2 size);
	~Billboard();
	std::vector<float3> ComputeQuad(const float3 cameraPosition, aiVector3D particlePosition);
	void SetSize(float2 value);

private:
	float2 size = { 1, 1 };
};


#endif // !BILLBOARD_H