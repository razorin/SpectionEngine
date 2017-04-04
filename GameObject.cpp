#include "Globals.h"
#include "Application.h"
#include "ModuleSceneManager.h"
#include "ModuleCamera.h"
#include "LightsManager.h"
#include "Light.h"
#include "ModuleGUI.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentAnim.h"
#include "ComponentCamera.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentParticleSystem.h"
#include "ComponentScript.h"
#include "ComponentTransform.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"



GameObject::GameObject(std::string id, GameObject * parent, const char * name, bool editableName) : id(id), parent(parent), name(name), editableName(editableName)
{
	AddComponent(ComponentType::COMPONENT_TYPE_TRANSFORM);
	AssignTransform();
	if (parent != nullptr)
	{
		transform->SetParent(parent->transform->GlobalTransform());
	}
}

GameObject::GameObject(std::string id, GameObject * parent, const char * name, bool editableName, const float3 & position, const float3 & scale, const Quat & rotation) :
	id(id), parent(parent), name(name), editableName(editableName)
{
	AddComponent(ComponentType::COMPONENT_TYPE_TRANSFORM);
	AssignTransform();
	transform->SetPosition(position);
	transform->SetScale(scale);
	transform->SetRotation(rotation);
	transform->CalculateLocalT();
	if (parent != nullptr)
	{
		transform->SetParent(parent->transform->GlobalTransform());
	}
}


GameObject::~GameObject()
{
	CleanUp();
}

void GameObject::AssignTransform()
{
	transform = (ComponentTransform*)FindComponent(ComponentType::COMPONENT_TYPE_TRANSFORM);
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject * parentGO)
{
	//We have 2 cases. We may have already a parent or not
	//Also if we dont want a parent anymore, parentGo will be nullptr

	float4x4 newParentGT = float4x4::identity;
	if (parentGO != nullptr) {
		newParentGT = parentGO->transform->GlobalTransform();
	}

	if (parent == nullptr) {
		transform->SetParent(newParentGT);
	}
	else {
		transform->ChangeParent(newParentGT);
	}

	parent = parentGO;
}

void GameObject::OnTransformChange()
{
	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		(*it)->transform->SetParent(transform->GlobalTransform());
	}
}


Component * GameObject::AddComponent(const ComponentType &type, ...)
{
	++componentCounter;
	Component *result = nullptr;
	Light* light = nullptr;
	std::map<ComponentType, int>::iterator it = componentCounterByType.find(type);

	//Init counter for this type
	if (it == componentCounterByType.end())
		componentCounterByType[type] = 0;

	//if (result->maxComponentsByGO != 0 && componentCounterByType[type] > result->maxComponentsByGO) {
	//	return result;
	//}

	switch (type) {
	case ComponentType::COMPONENT_TYPE_ANIMATION:
		result = new ComponentAnim(this, std::to_string(componentCounter));
		result->maxComponentsByGO = 1;
		break;
	case ComponentType::COMPONENT_TYPE_CAMERA:
		result = new ComponentCamera(this, std::to_string(componentCounter));
		result->maxComponentsByGO = 1;
		break;
	case ComponentType::COMPONENT_TYPE_LIGHT:
		light = App->lightsManager->AddLight(LT_POINT_LIGHT, { 0.0f, 5.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		if (light != nullptr) {
			result = new ComponentLight(this, std::to_string(componentCounter), light);
			result->maxComponentsByGO = 0;
		}
		break;
	case ComponentType::COMPONENT_TYPE_MATERIAL:
		result = new ComponentMaterial(this, std::to_string(componentCounter));
		result->maxComponentsByGO = 0;
		break;
	case ComponentType::COMPONENT_TYPE_MESH:
		result = new ComponentMesh(this, std::to_string(componentCounter));
		result->maxComponentsByGO = 1;
		break;
	case ComponentType::COMPONENT_TYPE_PARTICLE_SYSTEM:
		result = new ComponentParticleSystem(this, std::to_string(componentCounter));
		static_cast<ComponentParticleSystem*>(result)->Init(70, { 5,5 }, 50, 4, "Models/Particles/rainSprite.tga", { 0.15f,0.15f });
		result->maxComponentsByGO = 10;
		break;
	case ComponentType::COMPONENT_TYPE_SCRIPT:
		result = new ComponentScript(this, std::to_string(componentCounter));
		result->maxComponentsByGO = 10;
		break;
	case ComponentType::COMPONENT_TYPE_TRANSFORM:
		result = new ComponentTransform(this, std::to_string(componentCounter));
		result->maxComponentsByGO = 1;
		break;
	}

	if (result != nullptr) {
		++componentCounterByType[type];
		components.push_back(result);
	}

	return result;
}

bool GameObject::RemoveComponent(Component* component)
{
	bool found = false;
	//Find Component
	for (std::list<Component *>::iterator it = components.begin(); it != components.end();) {
		if (component == (*it)) {
			--componentCounterByType[(*it)->type];
			RELEASE(*it);
			components.erase(it);
			found = true;
			break;
		}
		else {
			++it;
		}
	}

	return found;

}

bool GameObject::RemoveChild(GameObject * go)
{
	bool ret = false;
	for (std::list<GameObject *>::iterator it = childs.begin(); it != childs.end(); ++it) {
		if ((*it) == go) {
			RELEASE((*it));
			childs.erase(it);
			ret = true;
			break;
		}
	}
	return ret;
}

std::list<Component*> * GameObject::FindComponents(const ComponentType & type) {
	//Delete list not members!!!!
	std::list<Component*> *result = new std::list<Component*>();
	int typeCounter = componentCounterByType[type];

	if (typeCounter == 0)
		return result;

	for (std::list<Component *>::iterator it = components.begin(); it != components.end(); ++it) {
		if ((*it)->type == type) {
			result->push_back((*it));
			if (result->size() == typeCounter)
				break;
		}
	}

	return result;
}

Component * GameObject::FindComponent(const ComponentType & type)
{
	Component* componentFound = nullptr;
	for (std::list<Component *>::iterator it = components.begin(); (it != components.end() && (componentFound == nullptr)); it++) {
		if ((*it)->type == type) {
			componentFound = *it;
		}
	}
	return componentFound;
}


GameObject * GameObject::FindGOByName(const std::string & name)
{
	GameObject* ret = nullptr;
	for (std::list<GameObject *>::iterator it = childs.begin(); it != childs.end(); ++it) {
		if ((*it)->name == name) {
			ret = (*it);
			break;
		}
		else {
			ret = (*it)->FindGOByName(name);
			if (ret != nullptr) {
				break;
			}
		}
	}
	return ret;
}

GameObject * GameObject::FindGoInChilds(const char * name)
{
	GameObject * goFound = nullptr;
	bool found = false;

	for (std::list<GameObject *>::iterator it = childs.begin(); (it != childs.end() && !found); it++) {
		if ((*it)->name.compare(name) == 0) {
			goFound = *it;
			found = true;
		}
		else
		{
			goFound = (*it)->FindGoInChilds(name);
			if (goFound != nullptr) {
				found = true;
			}
		}
	}
	return goFound;
}


void GameObject::Draw() const
{
	glPushMatrix();

	float4x4 globalTransform = transform->GlobalTransform();

	glMultMatrixf(globalTransform.Transposed().ptr());

	for (std::list<Component*>::const_iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->IsActive()) {
			//Camera
			if ((*it)->type == ComponentType::COMPONENT_TYPE_CAMERA)
			{
				static_cast<ComponentCamera*>(*it)->DrawFrustum();
			}
			//Meshes
			else if ((*it)->type == ComponentType::COMPONENT_TYPE_MESH)
			{
				// Apply frustum culling only if option activated and object is static
				if (this->staticObject) { //&& App->camera->GetActiveCamera()->IsFrustumCulling()) {
					bool contained = false;
					// Test all AABBs from this GO
					for (auto itBoxes = AABBoxes.begin(); itBoxes != AABBoxes.end(); ++itBoxes) {
						if (App->camera->GetActiveCamera()->ContainsAaBox((*itBoxes))) {
							contained = true;
							break;
						}
					}
					// Draw mesh if its inside the frustum
					if (contained) {
						static_cast<ComponentMesh*>(*it)->DrawMesh();
						//App->gui->console.AddLog("PRINTING MESH %s", this->name.c_str());
					}
					else {
						//App->gui->console.AddLog("NOT PRINTING MESH %s", this->name.c_str());
					}
				}
				else {
					// Draw mesh
					static_cast<ComponentMesh*>(*it)->DrawMesh();
					//App->gui->console.AddLog("PRINTING MESH %s", this->name.c_str());
				}

			}

			//Particle System
			if ((*it)->type == ComponentType::COMPONENT_TYPE_PARTICLE_SYSTEM)
			{
				static_cast<ComponentParticleSystem*>(*it)->Draw();
			}
			if ((*it)->type == ComponentType::COMPONENT_TYPE_ANIMATION)
			{
				ComponentAnim* componentAnim = (ComponentAnim*)(*it);

				if (!componentAnim->isPlaying)
					break;

				AnimInstance* instance = App->animator->GetInstance(componentAnim->instanceId);

				if (instance == nullptr)
				{
					break;
				}

				//instance->time += dt;
				//if (instance->time > instance->animation->duration)
				//{
				//	if (instance->loop)
				//	{
				//		while (instance->time > instance->animation->duration)
				//		{
				//			instance->time -= instance->animation->duration;
				//		}
				//	}
				//	else
				//		return;
				//}

				//float3 pos = float3::zero;
				//Quat rot = Quat::identity;

				//for (int i = 0; i < instance->animation->numChannels; i++)
				//{
				//	GameObject* boneGO = (*it)->FindGoInChilds(instance->animation->channels[i].name.data);
				//	if (boneGO != nullptr)
				//	{
				//		pos = float3::zero;
				//		rot = Quat::identity;

				//		GetTransform(instance, &instance->animation->channels[i], pos, rot);

				//		boneGO->transform->SetTransform(pos, rot);
				//	}
				//}
				//DeformGOMeshes(*it);

			}


		}
	}

	// Bounding Boxes
	DrawBoundingBoxes();

	glPopMatrix();

	for (std::list<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); it++)
	{
		(*it)->Draw();
	}
}

void GameObject::Update(float dt)
{
	for (std::list<Component*>::const_iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->IsActive()) {

			if ((*it)->type == ComponentType::COMPONENT_TYPE_ANIMATION)
			{
				ComponentAnim* componentAnim = (ComponentAnim*)(*it);

				if (!componentAnim->isPlaying)
					break;

				AnimInstance* instance = App->animator->GetInstance(componentAnim->instanceId);

				if (instance == nullptr)
				{
					break;
				}

				instance->time += dt;
				if (instance->time > instance->animation->duration)
				{
					if (instance->loop)
					{
						while (instance->time > instance->animation->duration)
						{
							instance->time -= instance->animation->duration;
						}
					}
					else
						return;
				}

				float3 pos = float3::zero;
				Quat rot = Quat::identity;

				for (int i = 0; i < instance->animation->numChannels; i++)
				{
					GameObject* boneGO = FindGoInChilds(instance->animation->channels[i].name.data);
					if (boneGO != nullptr)
					{
						pos = float3::zero;
						rot = Quat::identity;

						App->animator->GetTransform(instance, &instance->animation->channels[i], pos, rot);

						boneGO->transform->SetTransform(pos, rot);
					}
				}
				//TODO:REFACTOR
				App->animator->DeformGOMeshes(this);

			}

			//Particle System
			if ((*it)->type == ComponentType::COMPONENT_TYPE_PARTICLE_SYSTEM)
			{
				static_cast<ComponentParticleSystem*>(*it)->Update({ 0,0,5 });
			}
		}
	}
	for (std::list<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); it++)
	{
		(*it)->Update(dt);
	}

}

bool GameObject::CleanUp()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end();)
	{
		RELEASE(*it);
		it = components.erase(it);
	}

	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end(); )
	{
		RELEASE(*it);
		it = childs.erase(it);
	}
	return false;
}

void GameObject::AddComponentMesh(Mesh * mesh) {
	ComponentMesh* cmesh = (ComponentMesh*)AddComponent(ComponentType::COMPONENT_TYPE_MESH);
	cmesh->mesh = mesh;
	math::AABB AABBox;
	AABBox.SetNegativeInfinity();
	AABBox.Enclose((float3*)cmesh->mesh->vertices, cmesh->mesh->numVertices);
	AABBoxes.push_back(AABBox);
}

void GameObject::DrawBoundingBoxes() const {

	for (std::list<math::AABB>::const_iterator it = AABBoxes.begin(); it != AABBoxes.end(); ++it) {
		math::vec AABBoxPoints[8];
		(*it).GetCornerPoints(AABBoxPoints);
		glColor4f(1, 0, 1, 1);
		glBegin(GL_LINES);
		glVertex3f(
			AABBoxPoints[0].x,
			AABBoxPoints[0].y,
			AABBoxPoints[0].z);
		glVertex3f(
			AABBoxPoints[1].x,
			AABBoxPoints[1].y,
			AABBoxPoints[1].z);
		glVertex3f(
			AABBoxPoints[0].x,
			AABBoxPoints[0].y,
			AABBoxPoints[0].z);
		glVertex3f(
			AABBoxPoints[2].x,
			AABBoxPoints[2].y,
			AABBoxPoints[2].z);
		glVertex3f(
			AABBoxPoints[0].x,
			AABBoxPoints[0].y,
			AABBoxPoints[0].z);
		glVertex3f(
			AABBoxPoints[4].x,
			AABBoxPoints[4].y,
			AABBoxPoints[4].z);
		glVertex3f(
			AABBoxPoints[5].x,
			AABBoxPoints[5].y,
			AABBoxPoints[5].z);
		glVertex3f(
			AABBoxPoints[1].x,
			AABBoxPoints[1].y,
			AABBoxPoints[1].z);
		glVertex3f(
			AABBoxPoints[5].x,
			AABBoxPoints[5].y,
			AABBoxPoints[5].z);
		glVertex3f(
			AABBoxPoints[4].x,
			AABBoxPoints[4].y,
			AABBoxPoints[4].z);
		glVertex3f(
			AABBoxPoints[5].x,
			AABBoxPoints[5].y,
			AABBoxPoints[5].z);
		glVertex3f(
			AABBoxPoints[7].x,
			AABBoxPoints[7].y,
			AABBoxPoints[7].z);
		glVertex3f(
			AABBoxPoints[3].x,
			AABBoxPoints[3].y,
			AABBoxPoints[3].z);
		glVertex3f(
			AABBoxPoints[2].x,
			AABBoxPoints[2].y,
			AABBoxPoints[2].z);
		glVertex3f(
			AABBoxPoints[3].x,
			AABBoxPoints[3].y,
			AABBoxPoints[3].z);
		glVertex3f(
			AABBoxPoints[7].x,
			AABBoxPoints[7].y,
			AABBoxPoints[7].z);
		glVertex3f(
			AABBoxPoints[3].x,
			AABBoxPoints[3].y,
			AABBoxPoints[3].z);
		glVertex3f(
			AABBoxPoints[1].x,
			AABBoxPoints[1].y,
			AABBoxPoints[1].z);
		glVertex3f(
			AABBoxPoints[6].x,
			AABBoxPoints[6].y,
			AABBoxPoints[6].z);
		glVertex3f(
			AABBoxPoints[4].x,
			AABBoxPoints[4].y,
			AABBoxPoints[4].z);
		glVertex3f(
			AABBoxPoints[6].x,
			AABBoxPoints[6].y,
			AABBoxPoints[6].z);
		glVertex3f(
			AABBoxPoints[2].x,
			AABBoxPoints[2].y,
			AABBoxPoints[2].z);
		glVertex3f(
			AABBoxPoints[6].x,
			AABBoxPoints[6].y,
			AABBoxPoints[6].z);
		glVertex3f(
			AABBoxPoints[7].x,
			AABBoxPoints[7].y,
			AABBoxPoints[7].z);
		glEnd();
		glColor4f(1, 1, 1, 1);
	}
}

std::string GameObject::GetID() {
	return id;
}

std::string GameObject::GetName() {
	return name;
}

void GameObject::SetName(std::string value, bool editable) {
	editableName = editable;
	name = std::string(value);
}

bool GameObject::IsEditableName() {
	return editableName;
}

bool GameObject::IsToDelete() {
	return toDelete;
}

void GameObject::SetToDelete(bool value) {
	toDelete = value;
}

bool GameObject::IsStatic()
{
	return staticObject;
}

void GameObject::SetStatic(bool value)
{
	staticObject = value;
}

bool GameObject::IsSelected()
{
	return selectedGO;
}

void GameObject::SetSelected(bool value)
{
	selectedGO = value;
}

void GameObject::DrawGUIPanel() {
	const char* items[] = { "ANIMATION", "CAMERA", "LIGHT", "MATERIAL", "MESH", "PARTICLE SYSTEM", "SCRIPT" };
	int componentType = newComponentType;
	// GameObject Name
	if (editableName) {
		const int maxInput = 255;
		char inputName[maxInput + 1];
		strncpy(inputName, name.c_str(), maxInput);				// Copy all to maxInput, zero-padding if shorter
		inputName[maxInput] = '\0';								// Terminate with Null

		ImGuiInputTextFlags inputFlags = 0;
		inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;

		if (ImGui::InputText("", &*inputName, 255, inputFlags)) {
			if (inputName[0] == NULL || inputName[0] == ' ') {	//Give a default value in case of empty string
				inputName[0] = '0';
			}
			name = inputName;
		}
	}
	else {
		ImGui::Text(this->name.c_str());
	}
	ImGui::SameLine();
	// Static checkbox
	std::string checkboxLabel = "Static##" + id;
	ImGui::Checkbox(checkboxLabel.c_str(), &staticObject);
	ImGui::SameLine();
	// Remove
	std::string goLabel = "Remove##" + this->name;
	if (ImGui::Button(goLabel.c_str())) {
		GetParent()->RemoveChild(this);
		App->gui->ClearSelection();
	}
	else {
		for (std::list<Component *>::iterator it = components.begin(); it != components.end(); ) {
			if (!(*it)->IsToDelete()) {
				(*it)->DrawGUI();
				++it;
			}
			else {
				RELEASE(*it);
				it = components.erase(it);
			}
		}
		// Add component
		ImGui::Separator();
		std::string comboLabel = "##" + id;
		if (ImGui::Combo(comboLabel.c_str(), &componentType, items, IM_ARRAYSIZE(items))) {
			newComponentType = static_cast<ComponentType>(componentType);
		}
		ImGui::SameLine();
		if (ImGui::Button("Add Component")) {
			AddComponent(newComponentType);
		}
	}
}