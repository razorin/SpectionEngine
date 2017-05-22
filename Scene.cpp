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
#include "Mesh.h"
#include "ProgramManager.h";

using namespace std;

Scene::Scene()
{
	root = AddGameObject(nullptr, false, "Root");
}

Scene::~Scene()
{
}

GameObject* Scene::AddGameObject(GameObject* parent, bool editable, const std::string &name)
{
	++gameObjectsCounter;
	GameObject* go = new GameObject(std::to_string(gameObjectsCounter), parent, name == "" ? ("Empty Game Object###" + std::to_string(gameObjectsCounter)).c_str() : name.c_str(), editable);
	if (parent != nullptr) {
		parent->childs.push_back(go);
	}
	return go;
}

GameObject * Scene::GetGameObject(std::string name)
{
	return root->FindGOByName(name);
}

void Scene::LoadLevel(const char * path, const char * file, bool dynamic)
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

		mesh->InitializeBuffers(dynamic);

		// Save bones info
		if (aiMesh->HasBones())
		{
			mesh->numBones = aiMesh->mNumBones;
			mesh->bones = new Bone[mesh->numBones];
			for (int j = 0; j < mesh->numBones; j++)
			{
				mesh->bones[j].name = aiMesh->mBones[j]->mName;

				//mesh->bones[j].bind = aiMesh->mBones[j]->mOffsetMatrix;
				for (int row = 0; row < 4; row++)
				{
					for (int col = 0; col < 4; col++)
					{
						mesh->bones[j].bind[row][col] = aiMesh->mBones[j]->mOffsetMatrix[row][col];
					}
				}



				mesh->bones[j].numWeights = aiMesh->mBones[j]->mNumWeights;
				mesh->bones[j].weights = new Weight[mesh->bones[j].numWeights];
				for (int k = 0; k < mesh->bones[j].numWeights; k++)
				{
					mesh->bones[j].weights[k].vertex = aiMesh->mBones[j]->mWeights[k].mVertexId;
					mesh->bones[j].weights[k].weight = aiMesh->mBones[j]->mWeights[k].mWeight;
				}
			}
		}

		meshes.push_back(mesh);
	}

	//Create Gameobjects recursively
	aiNode* rootNode = scene->mRootNode;
	GameObject *rootRelative = AddGameObject(root, false, rootNode->mName.data);
	RecursiveNodeRead(rootRelative, *rootNode, nullptr);
	BindBonesTransform();

	aiReleaseImport(scene);

	// TODO: Grid of billboardings (not finished)
	//for (int i = -20; i < 20; i++) {
	//	GameObject *go = AddGameObject(root, true);
	//	go->AddComponent(ComponentType::COMPONENT_TYPE_BILLBOARDING);
	//}
}

void Scene::RecursiveNodeRead(GameObject * go, aiNode & assimpNode, GameObject * parentGO)
{
	go->SetName(assimpNode.mName.data);

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
		++gameObjectsCounter;
		GameObject* childGO = new GameObject(std::to_string(gameObjectsCounter), nullptr, "", false);
		aiNode* aiChildNode = assimpNode.mChildren[i];
		RecursiveNodeRead(childGO, *aiChildNode, go);
		go->childs.push_back(childGO);
	}
}

//TODO fix function to accept the rootframe node 
void Scene::BindBonesTransform()
{
	ComponentTransform* ct = GetGameObject("RootFrame")->transform;
	//ComponentTransform* ct = GetGameObject("$ColladaAutoName$_0")->transform;

	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->relRoot = ct;
		for (int j = 0; j < meshes[i]->numBones; j++)
		{
			GameObject* boneReference = GetGameObject(meshes[i]->bones[j].name.data);

			if (boneReference != nullptr)
			{
				meshes[i]->bones[j].ownerGOTransform = boneReference->transform;
			}
		}
	}
}

void Scene::Draw()
{
	//for (std::list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end(); it++) {
	//	//If object is not in frustum...dont draw
	//	if (true) {
	//		DrawRecursively(*it);
	//	}
	//}
	//TransformHierarchy();

	//DrawRecursively(root);
	App->programManager->UseProgram("Second Shader");
	GLfloat value[3] = { 0.0f, 5.0f, 0.0f };
	GLint locUniform = glGetUniformLocation(App->programManager->GetProgramId("Second Shader"), "light_position");
	glUniform3fv(locUniform, 3, value);
	DLOG("%d", locUniform);
	root->Draw();
	App->programManager->UnuseProgram(); 
	DrawHierarchyNodes(root);
}

void Scene::Update(float dt)
{
	root->Update(dt);



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

void Scene::DrawHierarchyNodes(GameObject * go)
{
	glBegin(GL_LINES);
	//glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	DrawRecursively(go);

	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
}

void Scene::DrawRecursively(GameObject * go)
{
	//glBegin(GL_LINES);
	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++)
	{
		float3 parentPos = go->transform->Position(SpaceMode::SPACE_GLOBAL);
		float3 childPos = (*it)->transform->Position(SpaceMode::SPACE_GLOBAL);

		glVertex3f(parentPos.x, parentPos.y, parentPos.z);
		glVertex3f(childPos.x, childPos.y, childPos.z);

		DrawRecursively(*it);
	}
	//glEnd();
}
