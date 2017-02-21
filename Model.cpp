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
		imageNames = new uint[scene->mNumMaterials];
		//LOAD EN EL SCENE
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			const aiMaterial* material = scene->mMaterials[i];
			int a = 5;
			int texIndex = 0;
			aiString path;
			int texturesPerMaterial = material->GetTextureCount(aiTextureType_DIFFUSE);
			if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
			{
				string textureFile = path.data;
				string texturePath = folderPath + textureFile;
				
				imageNames[i] = App->textures->LoadTexture(texturePath.c_str(), true);
			}
		}

		//LOADING ONTO THE MESH LIST
		meshes = new Mesh[scene->mNumMeshes];
		numMeshes = scene->mNumMeshes;
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i];
			
			meshes[i].vertex = mesh->mVertices;
			meshes[i].normals = mesh->mNormals;
			meshes[i].uvs = mesh->mNormals;
			meshes[i].numFaces = mesh->mNumFaces;
			meshes[i].materialIndex = mesh->mMaterialIndex;
			//uint materialIndex = mesh->mMaterialIndex;

			meshes[i].faces = new Face[meshes[i].numFaces];
			for (int j = 0; j < meshes[i].numFaces; j++)
			{
				aiFace face = mesh->mFaces[j];
				//meshes[i].faces[j].numIndex = 3;
				//meshes[i].faces[j].index = uint(3);
				for (int k = 0; k < 3; k++)
				{
					//aiVector3D vertex = mesh->mVertices[face.mIndices[k]];
					meshes[i].faces[j].index[k] = face.mIndices[k];
					//aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
					//aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
				}

			}
		}
		//uint materialIndex = 5;
	}
}

void Model::Clear() 
{
}

void Model::Draw() {

	//for (int i = 0; i < scene->mNumMeshes; ++i) 
	//{
	//	glBegin(GL_TRIANGLES);
	//	aiMesh* mesh = scene->mMeshes[i];
	//	for (int j = 0; j < mesh->mNumVertices; ++j) 
	//	{
	//		aiVector3D normal = mesh->mNormals[j];
	//		glNormal3f(normal.x, normal.y, normal.z);
	//		aiVector3D vertex = mesh->mVertices[j];
	//		glVertex3f(vertex.x, vertex.y, vertex.z);
	//		//mesh->mTextureCoords[0][mesh->mFaces[j].mIndices[j]]

	//	}
	//	glEnd();
	//}

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		int numFaces = mesh->mNumFaces;
		uint materialIndex = mesh->mMaterialIndex;


		glBindTexture(GL_TEXTURE_2D, imageNames[materialIndex]);
		glBegin(GL_TRIANGLES);

		for (int j = 0; j < numFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			
			for (int k = 0; k < 3; k++)
			{
				aiVector3D vertex = mesh->mVertices[face.mIndices[k]];
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
				glNormal3f(normal.x, normal.y, normal.z);
				glTexCoord2f(uv.x, uv.y);
				glVertex3f(vertex.x, vertex.y, vertex.z);
			}

		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}



}
