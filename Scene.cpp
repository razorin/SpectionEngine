#include "Globals.h"
#include "Application.h"
#include "ModuleAnimation.h"
#include "Scene.h"
#include "GameObject.h"
#include "ModuleTextures.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "MemLeaks.h"

using namespace std;

Scene::Scene()
{
}

Scene::~Scene()
{
	gameobjects.clear();
}

void Scene::AddGameObject(ObjectType type)
{
	
}

void Scene::DeleteGameObject(std::string name)
{
}

GameObject * Scene::GetGameObject(std::string name)
{
	bool found = false;
	GameObject* gameObject = nullptr;
	for (std::list<GameObject *>::iterator it = gameobjects.begin(); (it != gameobjects.end() && found == false); it++) {
		if ((*it)->name.compare(name) == 0) {
			gameObject = *it;
			found = true;
		}
	}
	return gameObject;
}

void Scene::LoadLevel(const char * path, const char * file)
{
	aiString folderPath = aiString(path);
	aiString filePath = folderPath;
	filePath.Append(file);

	const aiScene* scene = aiImportFile(filePath.data,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);


	//Load textures
	textureIds = new uint[scene->mNumMaterials];
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* material = scene->mMaterials[i];
		aiString textureFileName;
		int texturesPerMaterial = material->GetTextureCount(aiTextureType_DIFFUSE);
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFileName) == AI_SUCCESS)
		{
			aiString texturePath = folderPath;
			texturePath.Append(textureFileName.data);
			textureIds[i] = App->textures->LoadTexture(texturePath);
		}
	}


	//Load meshes
	uint numMeshes = scene->mNumMeshes;
	for (int i = 0; i < numMeshes; ++i)
	{
		aiMesh* aiMesh = scene->mMeshes[i];
		Mesh* mesh = new Mesh();

		uint numVertices = aiMesh->mNumVertices;
		mesh->numVertices = numVertices;

		mesh->vertices = new float[numVertices * 3];
		memcpy(mesh->vertices, aiMesh->mVertices, sizeof(float) * 3 * numVertices);

		if (aiMesh->HasNormals())
		{
			mesh->normals = new float[numVertices * 3];
			memcpy(mesh->normals, aiMesh->mNormals, sizeof(float) * 3 * numVertices);
		}
		if (aiMesh->HasTextureCoords(0))
		{
			mesh->textureCoords = new float[numVertices * 3];
			memcpy(mesh->textureCoords, aiMesh->mTextureCoords[0], sizeof(float) * 3 * numVertices);
		}

		mesh->imageName = textureIds[aiMesh->mMaterialIndex];

		uint numFaces = aiMesh->mNumFaces;
		mesh->numIndices = numFaces * 3;
		mesh->indices = new uint[numFaces * 3];
		for (int j = 0; j < numFaces; j++)
		{
			aiFace aiFace = aiMesh->mFaces[j];
			memcpy(&mesh->indices[j * 3], aiFace.mIndices, sizeof(uint) * 3);
		}

		mesh->InitializeBuffers();
		meshes.push_back(mesh);
	}

	//Create Gameobjects recursively
	root = new GameObject();
	gameobjects.push_back(root);
	aiNode* rootNode = scene->mRootNode;
	RecursiveNodeRead(root, *rootNode, nullptr);

	aiReleaseImport(scene);
}

void Scene::RecursiveNodeRead(GameObject * go, aiNode & assimpNode, GameObject * parentGO)
{
	go->name = assimpNode.mName.data;

	aiVector3D aiPos;
	aiQuaternion aiRot;
	aiVector3D aiScale;
	assimpNode.mTransformation.Decompose(aiScale, aiRot, aiPos);
	float3 pos = float3(aiPos.x, aiPos.y, aiPos.z);
	Quat rot = Quat(aiRot.x, aiRot.y, aiRot.z, aiRot.w);
	float3 scale = float3(aiScale.x, aiScale.y, aiScale.z);
	go->transform->SetTransform(pos, scale, rot);
	go->SetParent(parentGO);

	if (assimpNode.mNumMeshes > 0)
	{
		for (int i = 0; i < assimpNode.mNumMeshes; i++)
		{
			go->AddComponentMesh(meshes[assimpNode.mMeshes[i]]);
		}
	}

	uint numChild = assimpNode.mNumChildren;
	for (int i = 0; i < numChild; i++)
	{
		GameObject* childGO = new GameObject();
		aiNode* aiChildNode = assimpNode.mChildren[i];
		RecursiveNodeRead(childGO, *aiChildNode, go);
		go->childs.push_back(childGO);
		this->gameobjects.push_back(childGO);
	}
}

void Scene::Draw()
{
	root->Draw();

	TransformHierarchy();
	DrawHierarchyNodes(*gameobjects.begin());
	
}

void Scene::DebugGOInfo(GameObject * go)
{
	ComponentTransform * CT = go->transform;
	float3 pos = CT->Position();
	float3 scale = CT->Position();
	Quat rot = CT->Rotation();
	float3 rotation = rot.ToEulerXYZ();

	DLOGS("Node:%s     Rotation X:%f    Y:%f    Z:%f  ", go->name.c_str(), rotation.x * RADTODEG, rotation.y* RADTODEG, rotation.z* RADTODEG);



	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); )
	{
		if (it == go->childs.begin())
			DLOGS("Childs of :%s", go->name.c_str());

		DebugGOInfo((*it));

		it++;
		if (it == go->childs.end())
			DLOGS("End of childs of :%s", go->name.c_str());
	}


}

bool Scene::CleanUp()
{
	RELEASE(textureIds);
	for (int i = 0; i < meshes.size(); i++)
	{
		RELEASE(meshes[i]);
	}

	root->CleanUp();
	RELEASE(root);

	return true;
	
}

void Scene::DrawHierarchyNodes(GameObject* go)
{
	if (go->GetParent() != nullptr) {
		float4 tempPos = { go->transform->Position().x, go->transform->Position().y, go->transform->Position().z, 1.0f };
		float4 position = go->transform->GlobalTransform() * tempPos;
		tempPos = { go->GetParent()->transform->Position().x, go->GetParent()->transform->Position().y, go->GetParent()->transform->Position().z, 1.0f };
		float4 parent_position = go->GetParent()->transform->GlobalTransform() * tempPos;
		glBegin(GL_LINES);
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(position.x, position.y, position.z);
		glVertex3f(parent_position.x, parent_position.y, parent_position.z);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnd();
	}
	for (list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); ++it) {
		DrawHierarchyNodes(*it);
	}

}

void Scene::TransformHierarchy() {
	for (std::map<string, Animation*>::iterator it = App->animations->animations.begin(); it != App->animations->animations.end(); ++it)
	{

		(*it).second;
		for (int i = 0; i < (*it).second->numChannels; i++) {
			maxFrames = (*it).second->channels[i].numFrames;
			GameObject* go = GetGameObject((*it).second->channels[i].nodeName.data());
			if (go != nullptr) {

				//Recalculate local and global transforms
				float3 position = (*it).second->channels[i].positionKeyFrames[frame];

				float3 rotation = (*it).second->channels[i].rotationKeyFrames[frame];
				Quat rotationQuat = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);

				float3 scale = (*it).second->channels[i].scalingKeyFrames[frame];

				go->transform->SetTransform(position, scale, rotationQuat);

			}
		}
	}
	frame++;
	if (frame == maxFrames) {
		frame = 0;
	}
}
