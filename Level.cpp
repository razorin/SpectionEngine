#include "Level.h"
#include "Mesh.h"
#include "Globals.h"
#include "assimp\vector3.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp\anim.h"


Level::Level()
{
}


Level::~Level()
{
}

void Level::Load(const char * path, const char * file)
{
	/*std::string folderPath = std::string(path);
	std::string filePath = path + std::string(file);

	scene = aiImportFile(filePath.c_str(), aiProcess_PreTransformVertices | aiProcess_Triangulate);

	if (!scene)
	{
		DLOG("Error when trying to load model");
	}
	else
	{
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
		meshes = new Mesh[scene->mNumMeshes];
		numMeshes = scene->mNumMeshes;

		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* aiMesh = scene->mMeshes[i];

			uint numVertices = aiMesh->mNumVertices;
			meshes[i].numVertices = numVertices;

			meshes[i].vertices = new float[numVertices * 3];
			memcpy(meshes[i].vertices, aiMesh->mVertices, sizeof(float) * 3 * numVertices);

			meshes[i].normals = new float[numVertices * 3];
			memcpy(meshes[i].normals, aiMesh->mNormals, sizeof(float) * 3 * numVertices);

			//Just one texture
			//meshes[i].textureCoords = new float[numVertices * 3];
			//memcpy(meshes[i].textureCoords, aiMesh->mTextureCoords[0], sizeof(float) * 3 * numVertices);

			//More than one texture
			//DLOG("El numero de texturas de esta mesh es: %d", aiMesh->GetNumUVChannels());
			meshes[i].numTextures = aiMesh->GetNumUVChannels();
			meshes[i].textureCoords = new float*[meshes[i].numTextures];
			for (int j = 0; j < meshes[i].numTextures; j++) {
				meshes[i].textureCoords[j] = new float[numVertices * 3];
				memcpy(meshes[i].textureCoords[j], aiMesh->mTextureCoords[j], sizeof(float) * 3 * numVertices);
			}

			meshes[i].imageName = imageNames[aiMesh->mMaterialIndex];

			uint numFaces = aiMesh->mNumFaces;

			meshes[i].numIndices = numFaces * 3;
			meshes[i].indices = new uint[numFaces * 3];
			for (int j = 0; j < numFaces; j++)
			{
				aiFace aiFace = aiMesh->mFaces[j];
				//assert(aiFace.mNumIndices == 3);
				memcpy(&meshes[i].indices[j * 3], aiFace.mIndices, sizeof(uint) * 3);
			}


			//After all info is on our mesh we can initialize VBOs
			meshes[i].InitializeBuffers();
		}
	}*/
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
