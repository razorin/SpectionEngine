#include "Level.h"
#include "Mesh.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
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


	//DLOG("%s", rootNode->mName.data);

	//for (int i = 0; i < rootNode->mNumChildren; i++)
	//{
	//	DLOG("%s is child of %s", rootNode->mChildren[i]->mName.data, rootNode->mName.data);
	//}

	//Load nodes
	RecursiveNodeRead(root, *rootNode, nullptr);

	//Create all the meshes
	for (int i = 0; i < root->childs.size(); i++) {
		for (int j = 0; j < root->childs[i]->meshes.size(); j++) {
			/*

			//We get all textures and put it on imageNames
			imageNames = new uint[scene->mNumMaterials];
			for (int i = 0; i < scene->mNumMaterials; i++)
			{
				const aiMaterial* material = scene->mMaterials[i];
				// Just one texture
				int texIndex = 0;
				aiString path;
				int texturesPerMaterial = material->GetTextureCount(aiTextureType_DIFFUSE);
				if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
				{
					string textureFile = path.data;
					string texturePath = folderPath + textureFile;
					imageNames[i] = App->textures->LoadTexture(texturePath.c_str());
				}
			}

			

			// we get all meshes info and put it on an Mesh array
			Mesh* mesh = new Mesh();
			aiMesh* aiMesh = scene->mMeshes[root->childs[i]->meshes[j]];

			uint numVertices = aiMesh->mNumVertices;
			mesh->numVertices = numVertices;

			mesh->vertices = new float[numVertices * 3];
			memcpy(mesh->vertices, aiMesh->mVertices, sizeof(float) * 3 * numVertices);

			mesh->normals = new float[numVertices * 3];
			memcpy(mesh->normals, aiMesh->mNormals, sizeof(float) * 3 * numVertices);

			//More than one texture
			mesh->numTextures = aiMesh->GetNumUVChannels();
			mesh->textureCoords = new float*[mesh->numTextures];
			for (int j = 0; j < mesh->numTextures; j++) {
				mesh->textureCoords[j] = new float[numVertices * 3];
				memcpy(mesh->textureCoords[j], aiMesh->mTextureCoords[j], sizeof(float) * 3 * numVertices);
			}

			mesh->imageName = imageNames[aiMesh->mMaterialIndex];

			uint numFaces = aiMesh->mNumFaces;

			mesh->numIndices = numFaces * 3;
			mesh->indices = new uint[numFaces * 3];
			for (int j = 0; j < numFaces; j++)
			{
				aiFace aiFace = aiMesh->mFaces[j];
				//assert(aiFace.mNumIndices == 3);
				memcpy(&mesh->indices[j * 3], aiFace.mIndices, sizeof(uint) * 3);
			}


			//After all info is on our mesh we can initialize VBOs
			mesh->InitializeBuffers();

			//Add Mesh
			meshes.push_back(*mesh);

			*/
		}
	}

	DLOG("El numero de meshes es: %d", meshes.size());

	//Create all the materials

	PrintNodeInfo();

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
	
//	if(imageNames != nullptr) RELEASE(imageNames);
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
