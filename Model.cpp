#include "Model.h"
#include "Globals.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Glew\include\GL\glew.h"

#include <string.h>

Model::Model()
{
	
}


Model::~Model()
{
}

void Model::Load(const char* path, const char* file) {
	this->path = path;
	std::string pathString = std::string(path);
	std::string fileString = std::string(file);
	std::string fileName = pathString + fileString;

	scene = aiImportFile(fileName.c_str(), aiProcess_PreTransformVertices | aiProcess_FlipUVs);
	
	if (scene) {
		

	}
}

void Model::Clear() {

}

void Model::Draw() {

	for (int i = 0; i < scene->mNumMeshes; ++i) 
	{
		glBegin(GL_TRIANGLES);
		aiMesh* mesh = scene->mMeshes[i];
		for (int j = 0; j < mesh->mNumVertices; ++j) 
		{
			aiVector3D normal = mesh->mNormals[j];
			glNormal3f(normal.x, normal.y, normal.z);
			aiVector3D vertex = mesh->mVertices[j];
			glVertex3f(vertex.x, vertex.y, vertex.z);

		}
		glEnd();
	}
}
