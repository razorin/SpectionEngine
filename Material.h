#ifndef MATERIAL_H
#define MATERIAL_H

#include "assimp\types.h"

class Material
{
public:
	Material();
	Material(const Material *material);
	virtual ~Material();

public:
	aiColor3D diffuse;
	aiColor3D specular;
	aiColor3D ambient;
	aiColor3D emissive;
	aiColor3D transparent;
	float shininess = 0.0f;
};



#endif // !MATERIAL_H

