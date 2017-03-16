#include "ModuleAnimation.h"

using namespace std;



ModuleAnimation::ModuleAnimation() : Module(true)
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

void ModuleAnimation::Load(const char * path, const char * file)
{
	string folderPath = string(path);
	string filePath = path + string(file);
	const aiScene* aiScene = aiImportFile(filePath.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	uint numAnimations = aiScene->mNumAnimations;

	for (int i = 0; i < numAnimations; i++)
	{
		aiAnimation* aiAnim = aiScene->mAnimations[i];
		Anim* anim = new Anim();

		anim->name = aiAnim->mName;
		anim->duration = aiAnim->mDuration;
		anim->numChannels = aiAnim->mNumChannels;
		anim->channels = new AnimChannel[anim->numChannels];

		for (int j = 0; j < anim->numChannels; j++)
		{
			aiNodeAnim* aiAnimChannel = aiAnim->mChannels[j];
			anim->channels[j].name = aiAnimChannel->mNodeName;

			uint numKeyframes = aiAnimChannel->mNumPositionKeys;
			anim->channels[j].numKeyframes = numKeyframes;

			anim->channels[j].positions = new aiVector3D[numKeyframes];
			anim->channels[j].rotations = new aiQuaternion[numKeyframes];
			anim->channels[j].scales = new aiVector3D[numKeyframes];

			for (int k = 0; k < numKeyframes; k++)
			{
				anim->channels[j].positions[k] = aiAnimChannel->mPositionKeys[i].mValue;
				anim->channels[j].rotations[k] = aiAnimChannel->mRotationKeys[i].mValue;
				anim->channels[j].scales[k] = aiAnimChannel->mScalingKeys[i].mValue;
			}
		}

		//We have the Animation fully copied. We add it to our Map
		animations[anim->name.data] = anim;
	}

	aiReleaseImport(aiScene);
}

bool ModuleAnimation::CleanUp()
{
	return true;
}

update_status ModuleAnimation::Update(float dt)
{
	return UPDATE_CONTINUE;
}

uint ModuleAnimation::Play(const char * goName)
{
	return uint();
}

void ModuleAnimation::Stop(uint instanceId)
{
}

void ModuleAnimation::BlendTo(uint instanceId, const char * name, uint blendTime)
{
}

bool ModuleAnimation::GetTransform(uint instanceId, const char * channel, aiVector3D & position, aiQuaternion & rotation)
{
	return false;
}



