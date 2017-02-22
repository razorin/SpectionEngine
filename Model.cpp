#include "Application.h"
#include "Model.h"
#include "ModuleTextures.h"
#include "Mesh.h"
#include "Globals.h"
#include "assimp\vector3.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp\anim.h"
#include "Glew\include\GL\glew.h"

using namespace std;

Model::Model()
{
	
}


Model::~Model()
{
}

void Model::Load(const char* path, const char* file) 
{
	string folderPath = string(path);
	string filePath = path + string(file);

	scene = aiImportFile(filePath.c_str(), aiProcess_PreTransformVertices);

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
			meshes[i].textureCoords = new float[numVertices * 3];
			memcpy(meshes[i].textureCoords, aiMesh->mTextureCoords[0], sizeof(float) * 3 * numVertices);

			meshes[i].imageName = imageNames[aiMesh->mMaterialIndex];
			
			uint numFaces = aiMesh->mNumFaces;

			meshes[i].numIndices = numFaces * 3;
			meshes[i].indices = new uint[numFaces * 3];
			for (int j = 0; j < numFaces; j++)
			{
				aiFace aiFace = aiMesh->mFaces[j];
				assert(aiFace.mNumIndices == 3);
				memcpy(&meshes[i].indices[j*3], aiFace.mIndices, sizeof(uint) * 3);
			}
			
			
			//After all info is on our mesh we can initialize VBOs
			meshes[i].InitializeBuffers();
		}
	}
}

void Model::Clear() 
{
}

void Model::Draw() 
{
	//for (int i = 0; i < scene->mNumMeshes; ++i)
	//{
	//	aiMesh* mesh = scene->mMeshes[i];
	//	int numFaces = mesh->mNumFaces;
	//	uint materialIndex = mesh->mMaterialIndex;


	//	glBindTexture(GL_TEXTURE_2D, imageNames[materialIndex]);
	//	glBegin(GL_TRIANGLES);

	//	for (int j = 0; j < numFaces; j++)
	//	{
	//		aiFace face = mesh->mFaces[j];
	//		
	//		for (int k = 0; k < 3; k++)
	//		{
	//			aiVector3D vertex = mesh->mVertices[face.mIndices[k]];
	//			aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
	//			aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
	//			glNormal3f(normal.x, normal.y, normal.z);
	//			glTexCoord2f(uv.x, uv.y);
	//			glVertex3f(vertex.x, vertex.y, vertex.z);
	//		}

	//	}
	//	glEnd();
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}

	for (int i = 0; i < numMeshes; ++i)
	{
		meshes[i].Draw();
	}
}
