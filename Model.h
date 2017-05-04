#ifndef _MODEL_H_
#define _MODEL_H_

#include <list>

class aiScene;
class Mesh;
class Material;

class Model
{
	const aiScene* scene = 0;
public:
	Model();
	~Model();

	void Load(const char* path, const char* file, bool dynamic = false);
	void Clear();
	void Draw();

public:
	//Reference to all textureVBO
	uint* imageNames = nullptr;
	Mesh* meshes = nullptr;
	Material *materials = nullptr;
	int numMaterials = 0;
	int numMeshes = 0;

};

#endif /* _MODEL_H_ */