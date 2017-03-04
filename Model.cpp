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
#include "Material.h"

using namespace std;

Model::Model()
{
	DLOG("New Model");
}


Model::~Model()
{


}

void Model::Load(const char* path, const char* file)
{
	string folderPath = string(path);
	string filePath = path + string(file);

	scene = aiImportFile(filePath.c_str(), aiProcess_PreTransformVertices | aiProcess_Triangulate);

	if (!scene)
	{
		DLOG("Error when trying to load model");
	}
	else
	{
		//We get all textures and put it on imageNames
		imageNames = new uint[scene->mNumMaterials];
		materials = new Material[scene->mNumMaterials];
		numMaterials = scene->mNumMaterials;
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			float shiness_strength = 0.0f;
			const aiMaterial* material = scene->mMaterials[i];
			material->Get(AI_MATKEY_COLOR_DIFFUSE, materials[i].diffuse);
			material->Get(AI_MATKEY_COLOR_SPECULAR, materials[i].specular);
			material->Get(AI_MATKEY_COLOR_AMBIENT, materials[i].ambient);
			material->Get(AI_MATKEY_COLOR_EMISSIVE, materials[i].emissive);
			material->Get(AI_MATKEY_COLOR_TRANSPARENT, materials[i].transparent);
			material->Get(AI_MATKEY_SHININESS, materials[i].shininess);

			material->Get(AI_MATKEY_SHININESS_STRENGTH, shiness_strength);

			materials[i].specular = materials[i].specular * shiness_strength;
			materials[i].shininess *= 128.0f;
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
			if (aiMesh->HasTextureCoords(0))
			{
				meshes[i].textureCoords = new float[numVertices * 3];
				memcpy(meshes[i].textureCoords, aiMesh->mTextureCoords[0], sizeof(float) * 3 * numVertices);
			}

			//More than one texture
			//DLOG("El numero de texturas de esta mesh es: %d", aiMesh->GetNumUVChannels());
			//meshes[i].numTextures = aiMesh->GetNumUVChannels();
			//meshes[i].textureCoords = new float*[meshes[i].numTextures];
			//for (int j = 0; j < meshes[i].numTextures; j++) {
			//	meshes[i].textureCoords[j] = new float[numVertices * 3];
			//	memcpy(meshes[i].textureCoords[j], aiMesh->mTextureCoords[j], sizeof(float) * 3 * numVertices);
			//}

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
	}
}

void Model::Clear()
{
	RELEASE(imageNames);
	RELEASE_ARRAY(meshes);
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
	for (int i = 0; i < numMaterials; ++i) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, (float*)&materials[i].ambient);

		glMaterialfv(GL_FRONT, GL_DIFFUSE, (float*)&materials[i].diffuse);

		glMaterialfv(GL_FRONT, GL_SPECULAR, (float*)&materials[i].specular);

		glMaterialf(GL_FRONT, GL_SHININESS, materials[i].shininess);
	}
}
