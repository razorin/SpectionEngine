#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Globals.h"
#include <vector>
#include "assimp\vector3.h"
#include "assimp/scene.h"
#include "Mesh.h"
#include "MemLeaks.h"


struct Node {
	std::string name;
	aiVector3D position = aiVector3D(0,0,0);
	aiQuaternion rotation = aiQuaternion(1,0,0,0);
	aiVector3D scale = aiVector3D(1, 1, 1);
	std::vector<unsigned> meshes;
	Node* parent = nullptr;
	std::vector<Node*> childs;

	Node() {};
	~Node();
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
	void RecursiveNodeRead(Node* node, aiNode& aiNode, Node* parentNode);
	const void PrintNodeInfo();
	void Draw();
	void RecursiveNodeRelease(Node* node);
	void Clear();

	unsigned GetNumMeshes() const { return meshes.size(); }
	unsigned GetNumMaterials() const { return materials.size(); }

	Mesh& GetMesh(unsigned index) { return *meshes[index]; }
	//const Mesh& GetMesh(unsigned index) { return meshes[index]; }

	Node* GetRootNode() { return root; }
	//const Node* GetRootNode() { return root; }

	Node* FindNode(const char* name);
	void LinkNode(Node* node, Node* parent);

public:
	Node* root = nullptr;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	uint* imageNames;
};

#endif /* _LEVEL_H_ */