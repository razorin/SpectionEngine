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
#include "MathGeoLib\include\MathGeoLib.h"
#include "Application.h"
#include "ModuleCamera.h"

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

	//Load node hierarchy
	root = new Node();
	aiNode* rootNode = scene->mRootNode;
	RecursiveNodeRead(root, *rootNode, nullptr);
	RecursiveCalcTransforms(root);
	PrintNodeInfo(*root);


	//Load textures
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


	//Load meshes
	uint numMeshes = scene->mNumMeshes;
	for (int i = 0; i < numMeshes; ++i)
	{
		aiMesh* aiMesh = scene->mMeshes[i];
		Mesh* mesh = new Mesh();

		uint numVertices = aiMesh->mNumVertices;
		mesh->numVertices = numVertices;

		mesh->vertices = new float[numVertices * 3];
		memcpy(mesh->vertices, aiMesh->mVertices, sizeof(float) * 3 * numVertices);

		if (aiMesh->HasNormals())
		{
			mesh->normals = new float[numVertices * 3];
			memcpy(mesh->normals, aiMesh->mNormals, sizeof(float) * 3 * numVertices);
		}
		if (aiMesh->HasTextureCoords(0))
		{
			mesh->textureCoords = new float[numVertices * 3];
			memcpy(mesh->textureCoords, aiMesh->mTextureCoords[0], sizeof(float) * 3 * numVertices);
		}

		mesh->imageName = imageNames[aiMesh->mMaterialIndex];

		uint numFaces = aiMesh->mNumFaces;
		mesh->numIndices = numFaces * 3;
		mesh->indices = new uint[numFaces * 3];
		for (int j = 0; j < numFaces; j++)
		{
			aiFace aiFace = aiMesh->mFaces[j];
			memcpy(&mesh->indices[j * 3], aiFace.mIndices, sizeof(uint) * 3);
		}

		mesh->InitializeBuffers();
		meshes.push_back(mesh);
	}

	aiReleaseImport(scene);
}

void Level::RecursiveNodeRead(Node* node, aiNode& assimpNode, Node* parentNode)
{
	node->name = assimpNode.mName.data;

	aiVector3D aiPos;
	aiQuaternion aiRot;
	aiVector3D aiScale;
	assimpNode.mTransformation.Decompose(aiScale, aiRot, aiPos);
	node->position = float3 (aiPos.x, aiPos.y, aiPos.z);
	node->rotation = Quat (aiRot.x, aiRot.y, aiRot.z, aiRot.w);
	node->scale = float3(aiScale.x, aiScale.y, aiScale.z);

	//assimpNode.mTransformation.Decompose(node->scale, node->rotation, node->position);

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

void Level::RecursiveCalcTransforms(Node * node)
{
	float4x4 transform = float4x4::FromTRS(node->position, node->rotation, node->scale);
	if (node->parent != nullptr)
	{
		node->globalTransform = node->parent->globalTransform * transform;
	}
	else
	{
		node->globalTransform = transform;
	}

	for (int i = 0; i < node->childs.size(); i++)
	{
		RecursiveCalcTransforms(node->childs[i]);
	}
}

const void Level::PrintNodeInfo(Node & node)
{
	if (node.parent != nullptr)	{
		DLOG("%s is child of %s", node.name.c_str(), node.parent->name.c_str());
	}
	else {
		DLOG("%s", node.name.c_str());
	}

	//DLOG("x: %f,  y: %f,  z: %f", node.position.x, node.position.y, node.position.z);

	for (int i = 0; i < node.childs.size(); i++)
	{
		PrintNodeInfo(*node.childs[i]);
	}
}

//const void Level::PrintNodeInfo()
//{
//	DLOG("%s", root->name.c_str());
//	DLOG("x: %f,  y: %f,  z: %f", root->position.x, root->position.y, root->position.z);
//
//	for (int i = 0; i < root->childs.size(); i++)
//	{
//		DLOG("%s is child of %s", root->childs[i]->name.c_str(), root->name.c_str());
//		DLOG("x: %f,  y: %f,  z: %f", root->childs[i]->position.x, root->childs[i]->position.y, root->childs[i]->position.z);
//
//	}
//}


void Level::Draw(Node* node)
{
	glPushMatrix();
	glMultMatrixf(node->globalTransform.Transposed().ptr());

	//Position
	//glTranslatef(node.position.x, node.position.y, node.position.z);

	//Last we scale it
	//glScalef(node.scale.x, node.scale.y, node.scale.z);

	for (int i = 0; i < node->meshes.size(); i++)
	{
		meshes[node->meshes[i]]->Draw();
	}

	for (int i = 0; i < node->childs.size(); i++)
	{
		Draw(node->childs[i]);
	}

	glPopMatrix();
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
	RELEASE(imageNames);
	for (int i = 0; i < meshes.size(); i++)
	{
		RELEASE(meshes[i]);
	}
}

Node * Level::FindNode(const char * name)
{
	for (int i = 0; i < root->childs.size(); i++) {
		if (root->childs[i]->name.compare(name) == 0) {
			return root->childs[i];
		}
	}
	return nullptr;
}

void Level::LinkNode(Node * node, Node * parent)
{
	//Find position of the node in the parents child list
	int pos = -1;
	for (int i = 0; i < node->parent->childs.size(); i++) {
		if (node->parent->childs[i]->name.compare(node->parent->name) == 0) {
			pos = i;
		}
	}
	//Remove from the old parents child list
	node->parent->childs.erase(node->parent->childs.begin() + pos);
	//Assign new parent
	node->parent = parent;
	//Add to the new parents child list
	parent->childs.push_back(node);
}

Node::~Node()
{
}
