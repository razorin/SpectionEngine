#include "Model.h"

Model::Model()
{
}


Model::~Model()
{
}

void Model::Load(const char* file) {
	file = "Model/Batman/Batman.obj";

	scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene) {
		//get_bounding_box(&scene_min, &scene_max);
		//scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		//scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		//scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
	}
}

void Model::Clear() {

}

void Model::Draw() {

}
