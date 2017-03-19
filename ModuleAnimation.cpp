#include "ModuleAnimation.h"
#include "Application.h"
#include "ModuleSceneManager.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Scene.h"
#include "ComponentTransform.h"

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
	Load("Models/ArmyPilot/Animations/", "ArmyPilot_Run_Forwards.fbx");
	Play("Idle");
	//Play("Run_Forwards");


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
		anim->duration = (aiAnim->mDuration / aiAnim->mTicksPerSecond) * 500;
		anim->numChannels = aiAnim->mNumChannels;
		anim->channels = new NodeAnim[anim->numChannels];

		for (int j = 0; j < anim->numChannels; j++)
		{
			aiNodeAnim* aiAnimChannel = aiAnim->mChannels[j];
			anim->channels[j].name = aiAnimChannel->mNodeName;

			uint numKeyframes = aiAnimChannel->mNumPositionKeys;
			anim->channels[j].numKeyframes = numKeyframes;
			anim->channels[j].numPositions = numKeyframes;
			anim->channels[j].numRotations = numKeyframes;
			anim->channels[j].numScales = numKeyframes;

			anim->channels[j].positions = new float3[numKeyframes];
			anim->channels[j].rotations = new Quat[numKeyframes];
			anim->channels[j].scales = new float3[numKeyframes];

			for (int k = 0; k < numKeyframes; k++)
			{
				aiVector3D aiPos = aiAnimChannel->mPositionKeys[k].mValue;
				aiQuaternion aiRot = aiAnimChannel->mRotationKeys[k].mValue;
				aiVector3D aiScl = aiAnimChannel->mScalingKeys[k].mValue;
				anim->channels[j].positions[k] = float3(aiPos.x, aiPos.y, aiPos.z);
				anim->channels[j].rotations[k] = Quat(aiRot.x, aiRot.y, aiRot.z, aiRot.w);
				anim->channels[j].scales[k] = float3(aiScl.x, aiScl.y, aiScl.z);
			}
		}

		//We have the Animation fully copied. We add it to our Map
		animations[anim->name.data] = anim;
	}

	aiReleaseImport(aiScene);
}

bool ModuleAnimation::CleanUp()
{
	for (InstanceList::iterator it = instances.begin(); it != instances.end(); it++)
	{
		RELEASE(*it);
	}

	for (AnimationMap::iterator it = animations.begin(); it != animations.end(); it++)
	{
		for (int i = 0; i < (*it).second->numChannels; i++)
		{
			RELEASE_ARRAY((*it).second->channels[i].positions);
			RELEASE_ARRAY((*it).second->channels[i].rotations);
			RELEASE_ARRAY((*it).second->channels[i].scales);
		}
		RELEASE_ARRAY((*it).second->channels);
		RELEASE((*it).second);
	}

	return true;
}

//We update every instance time and blendTime if there's any
update_status ModuleAnimation::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Stop(instances[0]->id);
		Play("Run_Forwards");
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Stop(instances[0]->id);
		Play("Idle");
	}

	for (int i = 0; i < instances.size(); i++)
	{
		AnimInstance* instance = instances[i];
		instance->time += dt;
		while (instance->time > instance->animation->duration)
		{
			instance->time -= instance->animation->duration;
		}


		for (int j = 0; j < instance->animation->numChannels; j++)
		{
			GameObject* go = App->sceneManager->getCurrentScene()->GetGameObject(instance->animation->channels[j].name.data);
			if (go == nullptr)
			{
				int a = 1;
			}
			float3 pos = go->transform->Position();
			Quat rot = go->transform->Rotation();
			float3 scale = go->transform->Scale();
			GetTransform(instance->id, instance->animation->channels[j].name.data, pos, rot, scale);
			go->transform->SetTransform(pos, scale, rot);
		}
	}

	return UPDATE_CONTINUE;
}

//Play("Run_Forwards");
uint ModuleAnimation::Play(const char * animName)
{
	uint ret = -1;

	AnimationMap::iterator it = animations.find(animName);

	AnimInstance* animInstance = new AnimInstance();
	animInstance->animation = (*it).second;
	animInstance->time = 0;
	animInstance->loop = true;

	if (holes.size() == 0)
	{
		instances.push_back(animInstance);
		ret = instances.size() - 1;
	}
	else
	{
		ret = holes[0];
		instances[ret] = animInstance;
		holes.erase(holes.begin());
	}

	animInstance->id = ret;

	return ret;
}

void ModuleAnimation::Stop(uint instanceId)
{
	AnimInstance* animInstance = instances[instanceId];
	RELEASE(instances[instanceId]);
	holes.push_back(instanceId);
}

void ModuleAnimation::BlendTo(uint instanceId, const char * name, uint blendTime)
{
}

bool ModuleAnimation::GetTransform(uint instanceId, const char * channelName, float3& position, Quat& rotation, float3& scale)
{
	bool ret = false;


	AnimInstance *instance = instances[instanceId];

	uint channelIndex = -1;
	for (int i = 0; i < instance->animation->numChannels; i++)
	{
		if (strcmp(instance->animation->channels[i].name.data, channelName) == 0) {
			channelIndex = i;
			break;
		}
	}

	if (channelIndex == -1) {
		return ret;
	}

	NodeAnim channel = instance->animation->channels[channelIndex];

	float positionKey = float(instance->time * (channel.numPositions - 1)) / float(instance->animation->duration);
	float rotationKey = float(instance->time * (channel.numRotations - 1)) / float(instance->animation->duration);
	float scaleKey = float(instance->time * (channel.numScales - 1)) / float(instance->animation->duration);

	uint posIndex = uint(positionKey);
	uint rotIndex = uint(rotationKey);
	uint sclIndex = uint(scaleKey);

	if (posIndex >= channel.numKeyframes)
	{
		int a = 1;
	}

	float posLambda = positionKey - float(posIndex);
	float rotLambda = rotationKey - float(rotIndex);
	float sclLambda = scaleKey - float(sclIndex);

	//Next keyframe-Index to interpolate to
	uint nextIndex = posIndex + 1;
	if (nextIndex >= channel.numKeyframes)
	{
		nextIndex = 0;
	}

	//position = channel.positions[posIndex];
	//rotation = channel.rotations[rotIndex];
	//scale = channel.scales[sclIndex];

	//position = InterpVector3D(channel.positions[posIndex], channel.positions[nextIndex], posLambda);
	//rotation = InterpQuaternion(channel.rotations[rotIndex], channel.rotations[nextIndex], rotLambda);
	//scale = InterpVector3D(channel.scales[sclIndex], channel.scales[nextIndex], sclLambda);

	position = channel.positions[posIndex].Lerp(channel.positions[nextIndex], posLambda);
	rotation = channel.rotations[rotIndex].Lerp(channel.rotations[nextIndex], rotLambda);
	scale = channel.scales[sclIndex].Lerp(channel.scales[nextIndex], sclLambda);

	ret = true;
	return ret;
}

float3 ModuleAnimation::InterpVector3D(const float3 & first, const float3 & second, float lambda) const
{
	return first*(1 - lambda) + second*lambda;
}

Quat ModuleAnimation::InterpQuaternion(const Quat & first, const Quat & second, float lambda) const
{
	Quat result;

	float dot = first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
	if (dot >= 0.0f)
	{
		result.x = first.x*(1.0f - lambda) + second.x*lambda;
		result.y = first.y*(1.0f - lambda) + second.y*lambda;
		result.z = first.z*(1.0f - lambda) + second.z*lambda;
		result.w = first.w*(1.0f - lambda) + second.w*lambda;
	}
	else
	{
		result.x = first.x*(1.0f - lambda) - second.x*lambda;
		result.y = first.y*(1.0f - lambda) - second.y*lambda;
		result.z = first.z*(1.0f - lambda) - second.z*lambda;
		result.w = first.w*(1.0f - lambda) - second.w*lambda;
	}
	return result;
}
