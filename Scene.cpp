#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleTextures.h"
#include "ComponentTransform.h"


Scene::Scene()
{
}

Scene::~Scene()
{
	gameobjects.clear();
}

GameObject * Scene::GetGameObject(std::string name)
{
	bool found = false;
	GameObject* gameobject;
	for (std::list<GameObject *>::iterator it = gameobjects.begin(); it != gameobjects.end();) {
		if((*it)->name.compare(name) == 0) {
			gameobject = *it;
			found = true;
		}
		else {
			++it;
		}
	}
	if (found) {
		return gameobject;
	}
	else {
		return nullptr;
	}
}

void Scene::LoadLevel(const char * path, const char * file)
{
	//aiString folderPath = aiString(path);
	//aiString filePath = folderPath;
	//filePath.Append(file);

	//const aiScene* scene = aiImportFile(filePath.data,
	//	aiProcess_CalcTangentSpace |
	//	aiProcess_Triangulate |
	//	aiProcess_JoinIdenticalVertices |
	//	aiProcess_SortByPType);

	//root = new GameObject();

	//aiNode* rootNode = scene->mRootNode;
	//RecursiveNodeRead(root, *rootNode, nullptr);




















}

void Scene::RecursiveNodeRead(GameObject * go, aiNode & assimpNode, GameObject * parentGO)
{
	go->name = assimpNode.mName.data;

	aiVector3D aiPos;
	aiQuaternion aiRot;
	aiVector3D aiScale;
	assimpNode.mTransformation.Decompose(aiScale, aiRot, aiPos);

	go->transform->SetPosition(float3(aiPos.x, aiPos.y, aiPos.z));
	go->transform->SetRotation(Quat(aiRot.x, aiRot.y, aiRot.z, aiRot.w));
	go->transform->SetScale(float3(aiScale.x, aiScale.y, aiScale.z));

	if (assimpNode.mNumMeshes >0)
	{
		go->AddComponent(ComponentType::COMPONENT_TYPE_MESH);
	}






}







void Scene::Draw()
{
	//Draw Root GO
}
