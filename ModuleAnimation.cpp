#include "ModuleAnimation.h"
#include "Application.h"
#include "ModuleSceneManager.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentAnim.h"
#include "ComponentMesh.h"
#include "ModuleWindow.h"
#include "Brofiler\Brofiler.h"

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

	GameObject* go = App->sceneManager->getCurrentScene()->GetGameObject("$ColladaAutoName$_0");
	ComponentAnim* compAnim = (ComponentAnim*)go->AddComponent(ComponentType::COMPONENT_TYPE_ANIMATION);
	compAnim->Play("Idle");

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

			anim->channels[j].positions = new float3[numKeyframes];
			anim->channels[j].rotations = new Quat[numKeyframes];

			for (int k = 0; k < numKeyframes; k++)
			{
				aiVector3D aiPos = aiAnimChannel->mPositionKeys[k].mValue;
				aiQuaternion aiRot = aiAnimChannel->mRotationKeys[k].mValue;
				aiVector3D aiScl = aiAnimChannel->mScalingKeys[k].mValue;
				anim->channels[j].positions[k] = float3(aiPos.x, aiPos.y, aiPos.z);
				anim->channels[j].rotations[k] = Quat(aiRot.x, aiRot.y, aiRot.z, aiRot.w);
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
		}
		RELEASE_ARRAY((*it).second->channels);
		RELEASE((*it).second);
	}

	return true;
}

//We update every instance time and blendTime if there's any
update_status ModuleAnimation::Update(float dt)
{ BROFILER_CATEGORY("UpdateModuleAnimation", Profiler::Color::Orange)

	//Todo Change it so we can have more than 1 animation in the entire game
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		GameObject* go = App->sceneManager->getCurrentScene()->GetGameObject("$ColladaAutoName$_0");
		ComponentAnim* compAnim = (ComponentAnim*)go->FindComponent(ComponentType::COMPONENT_TYPE_ANIMATION);
		compAnim->Play("Run_Forwards");
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		GameObject* go = App->sceneManager->getCurrentScene()->GetGameObject("$ColladaAutoName$_0");
		ComponentAnim* compAnim = (ComponentAnim*)go->FindComponent(ComponentType::COMPONENT_TYPE_ANIMATION);
		compAnim->Play("Idle");
	}
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		GameObject* go = App->sceneManager->getCurrentScene()->GetGameObject("$ColladaAutoName$_0");
		ComponentAnim* compAnim = (ComponentAnim*)go->FindComponent(ComponentType::COMPONENT_TYPE_ANIMATION);
		if (compAnim->isPlaying)
		{
			compAnim->Stop();
		}
	}


	return UPDATE_CONTINUE;
}

void ModuleAnimation::DeformGOMeshes(GameObject * go)
{
	for (std::list<Component*>::iterator it = go->components.begin(); it != go->components.end(); it++)
	{
		if ((*it)->type == COMPONENT_TYPE_MESH)
			((ComponentMesh*)(*it))->mesh->Deform();
	}

	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++)
	{
		DeformGOMeshes(*it);
	}
}

AnimInstance* ModuleAnimation::GetInstance(unsigned id)
{
	AnimInstance* instance = nullptr;
	if (id < instances.size())
	{
		instance = instances[id];
	}
	return instance;
}

uint ModuleAnimation::Play(const char * animName, bool loop)
{
	AnimationMap::iterator it = animations.find(animName);
	return CreateAnimInstance((*it).second, loop);
}

uint ModuleAnimation::Play(Anim * anim, bool loop)
{
	return CreateAnimInstance(anim, loop);
}

uint ModuleAnimation::CreateAnimInstance(Anim * anim, bool loop)
{
	uint ret = -1;

	AnimInstance* animInstance = new AnimInstance();
	animInstance->animation = anim;
	animInstance->time = 0;
	animInstance->loop = loop;

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
	AnimInstance* animInstance = instances[instanceId];

	AnimationMap::iterator it = animations.find(name);
	unsigned nextInstanceId = CreateAnimInstance((*it).second, true);

	animInstance->next = instances[nextInstanceId];
	animInstance->blendDuration = blendTime;
}

bool ModuleAnimation::GetTransform(uint instanceId, const char * channelName, float3& position, Quat& rotation)
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

	uint posIndex = uint(positionKey);
	uint rotIndex = uint(rotationKey);

	float posLambda = positionKey - float(posIndex);
	float rotLambda = rotationKey - float(rotIndex);

	//Next keyframe-Index to interpolate to
	uint nextIndex = posIndex + 1;
	if (nextIndex >= channel.numKeyframes)
		nextIndex = 0;

	position = InterpVector3D(channel.positions[posIndex], channel.positions[nextIndex], posLambda);
	rotation = InterpQuaternion(channel.rotations[rotIndex], channel.rotations[nextIndex], rotLambda);

	if (instance->next != nullptr)
	{
		float3 blendingToPos = float3::zero;
		Quat blendingToRot = Quat::identity;
		GetTransform(instance->next->id, channel.name.data, blendingToPos, blendingToRot);
		float blendLambda = (float)instance->blendTime / instance->blendDuration;
		position = InterpVector3D(position, blendingToPos, blendLambda);
		rotation = InterpQuaternion(rotation, blendingToRot, blendLambda);
	}

	ret = true;
	return ret;
}

bool ModuleAnimation::GetTransform(AnimInstance * instance, NodeAnim * channel, float3 & position, Quat & rotation)
{
	float positionKey = float(instance->time * (channel->numPositions - 1)) / float(instance->animation->duration);
	float rotationKey = float(instance->time * (channel->numRotations - 1)) / float(instance->animation->duration);

	uint posIndex = uint(positionKey);
	uint rotIndex = uint(rotationKey);

	float posLambda = positionKey - float(posIndex);
	float rotLambda = rotationKey - float(rotIndex);

	//Next keyframe-Index to interpolate to
	uint nextIndex = posIndex + 1;
	if (nextIndex >= channel->numKeyframes)
		nextIndex = 0;

	position = InterpVector3D(channel->positions[posIndex], channel->positions[nextIndex], posLambda);
	rotation = InterpQuaternion(channel->rotations[rotIndex], channel->rotations[nextIndex], rotLambda);

	//if (instance->next != nullptr)
	//{
	//	float3 blendingToPos = float3::zero;
	//	Quat blendingToRot = Quat::identity;
	//	GetTransform(instance->next, channel, blendingToPos, blendingToRot);
	//	float blendLambda = (float)instance->blendTime / instance->blendDuration;
	//	position = InterpVector3D(position, blendingToPos, blendLambda);
	//	rotation = InterpQuaternion(rotation, blendingToRot, blendLambda);
	//}

	return true;
}

bool ModuleAnimation::GetBlendTransform(AnimInstance * blendSource, AnimInstance * blendTarget)
{
	return false;
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
	result.Normalize();
	return result;
}
