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

	root = new Node();
	aiNode* rootNode = scene->mRootNode;

	RecursiveNodeRead(root, *rootNode, nullptr);
	//PrintNodeInfo();

	aiReleaseImport(scene);
}

void Level::RecursiveNodeRead(Node* node, aiNode& assimpNode, Node* parentNode)
{
	node->name = assimpNode.mName.data;
	assimpNode.mTransformation.DecomposeNoScaling(node->rotation, node->position);
	for (int i = 0; i < assimpNode.mNumMeshes; i++)
	{
		node->meshes.push_back(assimpNode.mMeshes[i]);
	}
	node->parent = parentNode;

	uint numChild = assimpNode.mNumChildren;
	for (int i = 0; i < numChild; i++)
	{
		Node* childNode = new Node();
		aiNode* aiChildNode = assimpNode.mChildren[i];
		RecursiveNodeRead(childNode, *aiChildNode, node);
		node->childs.push_back(childNode);
	}
}

const void Level::PrintNodeInfo()
{
	DLOG("%s", root->name.c_str());

	for (int i = 0; i < root->childs.size(); i++)
	{
		DLOG("%s is child of %s", root->childs[i]->name.c_str(), root->name.c_str());
	}
}


void Level::Draw()
{

}

void Level::RecursiveNodeRelease(Node * node)
{
	for (int i = 0; i < node->childs.size(); i++)
	{
		RecursiveNodeRelease(node->childs[i]);
	}
	delete node;
}

void Level::Clear()
{
	RecursiveNodeRelease(root);
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
}
