#ifndef MESH_H
#define MESH_H

#include <list>
#include "Globals.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void InitializeBuffers();
	void Draw() const;
public:
	uint vboVertices = 0;
	uint vboIndices = 0;
	uint vboColors = 0;
	uint vboNormals = 0;
	uint *vboTextures = nullptr;

	uint numVertices = 0;
	uint numIndices = 0;
	uint numTextures = 0;

	uint *indices = nullptr;
	uint imageName;
	float *vertices = nullptr;
	float *colors = nullptr;
	float* normals = nullptr;
	//float *textureCoords = nullptr;
	float **textureCoords = nullptr;

};

#endif /* _MESH_H_ */

