#include "Level.h"
#include "Mesh.h"
#include "Globals.h"
#include "assimp\vector3.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp\anim.h"

using namespace std;

Level::Level()
{
}


Level::~Level()
{
}

void Level::Load(const char * path, const char * file)
{
	string folderPath = string(path);
	string filePath = path + string(file);
	const aiScene* scene = aiImportFile(filePath.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);


	aiNode* rootNode = scene->mRootNode;
	root = new Node();
	root->name = rootNode->mName.data;
	//Load position and rotation
	rootNode->mTransformation.DecomposeNoScaling(root->rotation, root->position);
	//Load meshes
	for (int i = 0; i < rootNode->mNumMeshes; i++)
	{
		//carga de meshes
		root->meshes.push_back(rootNode->mMeshes[i]);
	}

	uint numChilds = rootNode->mNumChildren;
	if (numChilds > 0)
	{
		for (int i = 0; i < numChilds; i++)
		{
			aiNode* childNode = rootNode->mChildren[i];

			RecursiveNodeRead(childNode, root);
		}
	}

	aiReleaseImport(scene);
}

void Level::RecursiveNodeRead(aiNode* node, Node* parent)
{
	Node* child = new Node();
	child->name = node->mName.data;
	node->mTransformation.DecomposeNoScaling(child->rotation, child->position);
	child->parent = parent;

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		//carga de meshes
		child->meshes.push_back(node->mMeshes[i]);
	}

	if (node->mNumChildren == 0) {
		root->childs.push_back(child);
	}
	else {
		for (int i = 0; i < node->mNumMeshes; i++) {
			RecursiveNodeRead(node->mChildren[i], child);
		}
		root->childs.push_back(child);
	}
}




void Level::Draw()
{

}

void Level::Clear()
{
}

Node * Level::FindNode(const char * name)
{
	return nullptr;
}

void Level::LinkNode(Node * node, Node * destination)
{
}

Node::~Node()
{
	DLOG("destructor de node");
	for (int i = 0; i < childs.size(); i++) {
		RELEASE(childs[i]);
	}
	RELEASE(parent);
}
