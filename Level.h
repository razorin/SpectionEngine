#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Globals.h"
#include <vector>
#include "assimp\vector3.h"
#include "assimp/scene.h"
#include "Mesh.h"

struct Node {
	std::string name;
	aiVector3D position = aiVector3D(0,0,0);
	aiQuaternion rotation = aiQuaternion(1,0,0,0);
	std::vector<unsigned> meshes;
	Node* parent = nullptr;
	std::vector<Node*> childs;
};

struct Material {
	aiColor4D ambient = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D specular = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
	unsigned num_vertices = 0;
	unsigned* indices;
	unsigned num_indices = 0;
};

class Level
{
public:
	Level();
	~Level();

	void Load(const char* path, const char* file);
	void Draw();
	void Clear();

	unsigned GetNumMeshes() const { return meshes.size(); }
	unsigned GetNumMaterials() const { return materials.size(); }

	Mesh& GetMesh(unsigned index) { return meshes[index]; }
	//const Mesh& GetMesh(unsigned index) { return meshes[index]; }

	Node* GetRootNode() { return root; }
	//const Node* GetRootNode() { return root; }

	Node* FindNode(const char* name);
	void LinkNode(Node* node, Node* destination);

public:
	Node* root = nullptr;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
};

#endif /* _LEVEL_H_ */