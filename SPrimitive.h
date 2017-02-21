#ifndef SPRIMITIVE_H
#define SPRIMITIVE_H

#include "Globals.h"

enum  SPRIMITIVE_TYPE{
		SCUBE_TYPE,
		SPLANE_TYPE,
		SCYLINDER_TYPE,
		UNDEFINED
};

class SPrimitive
{
public:
	SPrimitive();
	virtual ~SPrimitive();

	virtual void InitializeBuffers();
	virtual void Draw() const;

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
	bool toDelete = false;
	

	uint ImageName;

};




#endif // !SPRIMITIVE_H

