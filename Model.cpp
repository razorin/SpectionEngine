#include "Model.h"
#include "Globals.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

Model::Model()
{
	
}


Model::~Model()
{
}

void Model::Load(const char* file) {

	scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_FlipUVs);
	
	if (scene) {
		
	}
}

void Model::Clear() {

}

void Model::Draw() {

}
