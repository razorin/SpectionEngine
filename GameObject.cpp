#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentAnim.h"
#include "ComponentCamera.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
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


GameObject::GameObject(GameObject * parent, const char * name) : parent(parent), name(name)
{
	AddComponent(ComponentType::COMPONENT_TYPE_TRANSFORM);
	AssignTransform();
	if (parent != nullptr)
	{
		transform->SetParent(parent->transform->GlobalTransform());
	}
}

GameObject::GameObject(GameObject * parent, const char * name, const float3 & position, const float3 & scale, const Quat & rotation) :
	parent(parent), name(name)
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
	//for (std::list<GameObject *>::iterator it = childs.begin(); it != childs.end();) {
	//	delete (*it);
	//	it = childs.erase(it);
	//	++it;
	//}

	//for (std::list<Component *>::iterator it = components.begin(); it != components.end();) {
	//	delete (*it);
	//	it = components.erase(it);
	//	++it;
	//}
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


Component * GameObject::AddComponent(const ComponentType &type)
{
	Component *result = nullptr;
	std::map<ComponentType, int>::iterator it = componentCounter.find(type);

	//Init counter for this type
	if (it == componentCounter.end())
		componentCounter[type] = 0;

	//if (result->maxComponentsByGO != 0 && componentCounter[type] > result->maxComponentsByGO) {
	//	return result;
	//}

	switch (type) {
	case ComponentType::COMPONENT_TYPE_ANIMATION:
		result = new ComponentAnim(this);
		result->maxComponentsByGO = 1;
		break;
	case ComponentType::COMPONENT_TYPE_CAMERA:
		result = new ComponentCamera(this);
		result->maxComponentsByGO = 1;
		break;
	case ComponentType::COMPONENT_TYPE_LIGHT:
		result = new ComponentLight(this);
		result->maxComponentsByGO = 0;
		break;
	case ComponentType::COMPONENT_TYPE_MATERIAL:
		result = new ComponentMaterial(this);
		result->maxComponentsByGO = 0;
		break;
	case ComponentType::COMPONENT_TYPE_MESH:
		result = new ComponentMesh(this);
		result->maxComponentsByGO = 1;
		break;
	case ComponentType::COMPONENT_TYPE_SCRIPT:
		result = new ComponentScript(this);
		result->maxComponentsByGO = 0;
		break;
	case ComponentType::COMPONENT_TYPE_TRANSFORM:
		result = new ComponentTransform(this);
		result->maxComponentsByGO = 1;
		break;
	}

	if (result != nullptr) {
		++componentCounter[type];
		components.push_back(result);
	}

	return result;
}

bool GameObject::RemoveComponent(Component *component)
{
	bool found = false;
	//Find Component
	for (std::list<Component *>::iterator it = components.begin(); it != components.end();) {
		if (component == (*it)) {
			--componentCounter[(*it)->type];
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

std::list<Component*> * GameObject::FindComponents(const ComponentType & type) {
	//Delete list not members!!!!
	std::list<Component*> *result = new std::list<Component*>();
	int typeCounter = componentCounter[type];

	if (typeCounter == 0)
		return result;

	for (std::list<Component *>::iterator it = components.begin(); it != components.end(); it++) {
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


	//Textures

	//Lights

	//Materials

	//Meshes
	for (std::list<Component*>::const_iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == ComponentType::COMPONENT_TYPE_MESH)
		{
			ComponentMesh* cmesh = (ComponentMesh*)(*it);
			Mesh* mesh = cmesh->mesh;


			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
			glVertexPointer(3, GL_FLOAT, 0, NULL);



			if (mesh->normals != nullptr)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->vboNormals);
				glNormalPointer(GL_FLOAT, sizeof(float) * 3, 0);
			}

			if (mesh->colors != nullptr)
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->vboColors);
				glColorPointer(3, GL_FLOAT, 0, NULL);
			}

			if (mesh->textureCoords != nullptr)
			{
				//for (int i = 0; i < numTextures; i++) {
				//	glBindTexture(GL_TEXTURE_2D, imageName);
				//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				//	glBindBuffer(GL_ARRAY_BUFFER, vboTextures[i]);
				//	//Deberia estar fuera
				//	//glTexCoordPointer(3, GL_FLOAT, 0, NULL);
				//}
				glTexCoordPointer(3, GL_FLOAT, 0, NULL);
				glBindTexture(GL_TEXTURE_2D, mesh->imageName);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->vboTextures);
				glTexCoordPointer(3, GL_FLOAT, 0, NULL);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIndices);
			glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);


			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
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

bool GameObject::CleanUp()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		RELEASE(*it);
	}

	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		(*it)->CleanUp();
		RELEASE(*it);
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

void GameObject::DrawGUIPanel() const {
	ImGui::Text(this->name.c_str());
	for (std::list<Component *>::const_iterator it = components.begin(); it != components.end(); it++) {
		(*it)->DrawGUI();
	}
}