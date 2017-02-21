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
	uint vbo_vertices = 0;
	uint vbo_indices = 0;
	uint vbo_colors = 0;
	uint vbo_normals = 0;
	uint vbo_textures = 0;

	uint numVertices = 0;
	uint numIndices = 0;

	uint *indices = nullptr;
	float *vertices = nullptr;
	float *colors = nullptr;
	float* normals = nullptr;
	float *textureCoords = nullptr;

	uint ImageName;
};

#endif /* _MESH_H_ */

