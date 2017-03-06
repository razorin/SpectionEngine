#ifndef COMPONENTMESH_H
#define COMPONENTMESH_H

#include "Globals.h"
#include "Component.h"


class ComponentMesh :
	public Component
{
public:
	ComponentMesh(GameObject* container);
	~ComponentMesh();

	void InitializeBuffers();

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

};

#endif