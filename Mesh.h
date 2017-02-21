#ifndef MESH_H
#define MESH_H

#include <list>
#include "Globals.h"
#include "assimp\vector3.h"

//class aiVector3D;

struct Face
{
	uint index[3];
	int numIndex = 3;
};

class Mesh
{
public:
	Mesh();
	~Mesh();
public:
	aiVector3D* vertex;
	int numVertex;
	aiVector3D* uvs;
	int numUvs;
	aiVector3D* normals;
	int numNormals;
	Face* faces;
	int numFaces;
	uint materialIndex;
};

#endif /* _MESH_H_ */

