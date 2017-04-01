#ifndef MESH_H
#define MESH_H

#include <list>
#include "Globals.h"
#include "AssimpIncludes.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "ComponentTransform.h"



struct Weight
{
	unsigned vertex = 0;
	float weight = 0.0f;
};

struct Bone
{
	aiString name;
	Weight* weights = nullptr;
	unsigned numWeights = 0;
	float4x4 bind;
	ComponentTransform* ownerGOTransform;
};

class Mesh
{

public:
	Mesh();
	~Mesh();

	void InitializeBuffers();
	void Draw() const;
	void Deform();


public:
	uint vboVertices = 0;
	uint vboIndices = 0;
	uint vboColors = 0;
	uint vboNormals = 0;
	uint vboTextures = 0;

	uint numVertices = 0;
	uint numIndices = 0;
	uint numTextures = 0;

	uint *indices = nullptr;
	uint imageName;
	float *vertices = nullptr;
	float *colors = nullptr;
	float* normals = nullptr;
	float *textureCoords = nullptr;

	Bone* bones = nullptr;
	unsigned numBones = 0;

};

#endif /* _MESH_H_ */

