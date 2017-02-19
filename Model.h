#ifndef _MODEL_H_
#define _MODEL_H_

#include <list>

class aiScene;

class Model
{
	const aiScene* scene = 0;
public:
	Model();
	~Model();

	void Load(const char* path, const char* file);
	void Clear();
	void Draw();

public:
	//Reference to all textureVBO
	uint* imageNames;

};

#endif /* _MODEL_H_ */