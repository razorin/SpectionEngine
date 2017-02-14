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
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < scene->mNumMeshes; ++i) {
			int normalIndex = 0;
			for (int j = 0; j < scene->mMeshes[i]->mNumVertices; ++j) {
				aiVector3D vertexPosition = scene->mMeshes[i]->mVertices[j];
				glVertex3f(vertexPosition.x, vertexPosition.y, vertexPosition.z);
				if (j % 3 == 0) {
					aiVector3D normalVector = scene->mMeshes[i]->mNormals[normalIndex];
					glNormal3f(normalVector.x, normalVector.y, normalVector.z);
					++normalIndex;
				}
				if (j < 8) {
					aiVector3D *textureCoords = scene->mMeshes[i]->mTextureCoords[j];
					if (textureCoords != NULL) {
						glTexCoord2f(textureCoords->x, textureCoords->y);
					}
				}
			}
		}
	glEnd();
}
