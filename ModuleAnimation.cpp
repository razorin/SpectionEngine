#include "ModuleAnimation.h"
#include "assimp\vector3.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp\anim.h"
#include "MathGeoLib\include\MathGeoLib.h"

using namespace std;


ModuleAnimation::ModuleAnimation(const JSON_Object* json, bool active) : Module(json,active)
{
}


ModuleAnimation::~ModuleAnimation()
{
}

bool ModuleAnimation::Init()
{
	return true;
}

bool ModuleAnimation::Start() 
{
	Load("Models/ArmyPilot/Animations/", "ArmyPilot_Idle.fbx");
	return true;
}

update_status ModuleAnimation::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleAnimation::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleAnimation::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAnimation::CleanUp()
{
	for (std::map<string, Animation*>::iterator it = animations.begin(); it != animations.end(); ++it)
	{
		for (int i = 0; i < (*it).second->numChannels; i++) {
			RELEASE_ARRAY((*it).second->channels[i].positionKeyFrames);
			RELEASE_ARRAY((*it).second->channels[i].rotationKeyFrames);
			RELEASE_ARRAY((*it).second->channels[i].scalingKeyFrames);
		}
		RELEASE_ARRAY((*it).second->channels);
		delete (*it).second;
	}

	animations.clear();
	return true;
}

void ModuleAnimation::Load(const char * path, const char * file)
{
	string folderPath = string(path);
	string filePath = path + string(file);
	const aiScene* scene = aiImportFile(filePath.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	DLOG("Has animations: %s",scene->mAnimations[0]->mName.data);

	for (int i = 0; i < scene->mNumAnimations; i++) {
		Animation* anim = new Animation();
		anim->name = scene->mAnimations[i]->mName.data;
		anim->duration = scene->mAnimations[i]->mDuration;
		anim->numChannels = scene->mAnimations[i]->mNumChannels;
		anim->channels = new AnimationChannel[scene->mAnimations[i]->mNumChannels];
		for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
			anim->channels[j].nodeName = scene->mAnimations[i]->mChannels[j]->mNodeName.data;
			//Position Key Frames
			anim->channels[j].positionKeyFrames = new float3[scene->mAnimations[i]->mChannels[j]->mNumPositionKeys];
			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++) {
				anim->channels[j].positionKeyFrames[k].x = scene->mAnimations[i]->mChannels[j]->mPositionKeys->mValue.x;
				anim->channels[j].positionKeyFrames[k].y = scene->mAnimations[i]->mChannels[j]->mPositionKeys->mValue.y;
				anim->channels[j].positionKeyFrames[k].z = scene->mAnimations[i]->mChannels[j]->mPositionKeys->mValue.z;
			}
			//Rotation Key Frames
			anim->channels[j].rotationKeyFrames = new float3[scene->mAnimations[i]->mChannels[j]->mNumRotationKeys];
			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++) {
				anim->channels[j].rotationKeyFrames[k].x = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.x;
				anim->channels[j].rotationKeyFrames[k].y = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.y;
				anim->channels[j].rotationKeyFrames[k].z = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.z;
			}
			//Position Key Frames
			anim->channels[j].scalingKeyFrames = new float3[scene->mAnimations[i]->mChannels[j]->mNumScalingKeys];
			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++) {
				anim->channels[j].scalingKeyFrames[k].x = scene->mAnimations[i]->mChannels[j]->mScalingKeys->mValue.x;
				anim->channels[j].scalingKeyFrames[k].y = scene->mAnimations[i]->mChannels[j]->mScalingKeys->mValue.y;
				anim->channels[j].scalingKeyFrames[k].z = scene->mAnimations[i]->mChannels[j]->mScalingKeys->mValue.z;
			}
		}
		animations[scene->mAnimations[i]->mName.data] = anim;
	}

	DLOG("Size es: %d", animations["Idle"]->channels[0].positionKeyFrames->Size);
	DLOG("El primer nodo es %s", animations["Idle"]->channels[0].nodeName.data());
	//DLOG("El ultimo nodo es %s", animations["Idle"]->channels[63].nodeName.data());

	aiReleaseImport(scene);
}