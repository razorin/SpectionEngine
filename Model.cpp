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
				
				imageNames[i] = App->textures->LoadTexture(texturePath.c_str());
			}
		}


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
			meshes[i].textureCoords = new float[3];
			memcpy(meshes[i].textureCoords, aiMesh->mTextureCoords, sizeof(float) * 3);
			
			//With more than one texture
			/*meshes[i].textureCoords = new float*[8];
			for (int j = 0; j < 8; j++) {
				if (aiMesh->mTextureCoords[j] != NULL) {
					meshes[i].textureCoords[j] = new float[2];
					memcpy(meshes[i].textureCoords[j], aiMesh->mTextureCoords[j], sizeof(float) * 2);
					meshes[i].numTextures++;
				}
				else {
					meshes[i].textureCoords[j] = nullptr;
				}
				
			}*/
			

			//meshes[i].vertices = mesh->mvertices;
			//meshes[i].normals = mesh->mnormals;
			//meshes[i].uvs = mesh->mtexturecoords;
			//meshes[i].numfaces = mesh->mnumfaces;
			int l = 0;

			uint numFaces = aiMesh->mNumFaces;

			meshes[i].numIndices = numFaces * 3;
			meshes[i].indices = new uint[numFaces * 3];
			//meshes[i].faces = new face[meshes[i].numfaces];
			for (int j = 0; j < numFaces; j++)
			{
				aiFace aiFace = aiMesh->mFaces[j];

				assert(aiFace.mNumIndices == 3);
				
				memcpy(&meshes[i].indices[j*3], aiFace.mIndices, sizeof(uint) * 3);

				//meshes[i].faces[j].index = new unsigned int[aiMesh->mfaces[j].mnumindices];

				//for (int k = 0; k < 3; k++)
				//{
				//	//aivector3d vertex = mesh->mvertices[face.mindices[k]];
				//	meshes[i].faces[j].index[k] = face.mindices[k];
				//	meshes[i].index[l] = face.mindices[k];
				//	l++;
				//	meshes[i].numindex++;
				//	//aivector3d uv = mesh->mtexturecoords[0][face.mindices[k]];
				//	//aivector3d normal = mesh->hasnormals() ? mesh->mnormals[face.mindices[k]] : aivector3d(1.0f, 1.0f, 1.0f);
				//}

			}
			meshes[i].InitializeBuffers();
		}
		//uint materialindex = 5;
		//dlog("%f -> %f -> %f",meshes[1].vertex[1].x, meshes[1].vertex[1].y, meshes[1].vertex[1].z);
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
