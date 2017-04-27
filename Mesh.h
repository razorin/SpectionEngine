#ifndef MESH_H
#define MESH_H

#include <list>
#include "Globals.h"
#include "AssimpIncludes.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleSceneManager.h"
#include "Scene.h"



struct Weight
{
	unsigned vertex = 0;
	float weight = 0.0f;

	Weight(){
	}
	Weight(const Weight *weight) : vertex(weight->vertex), weight(weight->weight) {

	}
};

struct Bone
{
	aiString name;
	Weight* weights = nullptr;
	unsigned numWeights = 0;
	float4x4 bind;
	ComponentTransform* ownerGOTransform;

	Bone() {

	}
	Bone(const Bone *bone) : name(bone->name), numWeights(bone->numWeights)
	{
		weights = new Weight[numWeights];
		const GameObject *go = bone->ownerGOTransform->getGameObject();
		std::string name = go->GetName();
		GameObject *parent = App->sceneManager->getCurrentScene()->GetGameObject(name);
		ownerGOTransform = parent->transform;
		for (int i = 0; i < numWeights; ++i) {
			weights[i] = new Weight(bone->weights[i]);
		}

		bind = float4x4(bone->bind);
	}

	Bone& operator= (const Bone *bone) {
		name = aiString(bone->name);
		numWeights = bone->numWeights;
		weights = new Weight[numWeights];
		const GameObject *go = bone->ownerGOTransform->getGameObject();
		std::string name = go->GetName();
		GameObject *parent = App->sceneManager->getCurrentScene()->GetGameObject(name);
		ownerGOTransform = parent->transform;
		for (int i = 0; i < numWeights; ++i) {
			weights[i] = new Weight(bone->weights[i]);
		}
		bind = float4x4(bone->bind);

		return *this;
	}

	~Bone() {
		RELEASE_ARRAY(this->weights);
	}
};

class Mesh
{

public:
	Mesh();
	Mesh(Mesh* mesh);
	~Mesh();

	void InitializeBuffers();
	void Draw() const;
	void Deform();


public:
	uint vboVertices = 0;
	uint vboIndices = 0;
	uint vboColors = 0;
	uint vboNormals = 0;
	uint vboTextures = 0;

	uint numVertices = 0;
	uint numIndices = 0;
	//uint numTextures = 0;
	uint imageName = 0;

	uint *indices = nullptr;
	float *vertices = nullptr;
	float *colors = nullptr;
	float *normals = nullptr;
	float *textureCoords = nullptr;

	Bone *bones = nullptr;
	unsigned numBones = 0;

};

#endif /* _MESH_H_ */

