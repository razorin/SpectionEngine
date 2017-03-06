#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentScript.h"
#include "ComponentTransform.h"


//TODO delete empty constuctor.This one below can do the same
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
	if (parent != nullptr)
	{
		transform->SetParent(parent->transform->GlobalTransform());
	}
}


GameObject::~GameObject()
{
	for (std::list<GameObject *>::iterator it = childrens.begin(); it != childrens.end();) {
		delete (*it);
		it = childrens.erase(it);
		++it;
	}

	for (std::list<Component *>::iterator it = components.begin(); it != components.end();) {
		delete (*it);
		it = components.erase(it);
		++it;
	}
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
	transform->SetParent(parentGO->transform->GlobalTransform());
	//ALLWAYS change parent after changing transform values. It requires its older parent to recalculate its new localTransform;
	parent = parentGO;
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
	for (std::list<Component *>::iterator it = components.begin(); (it != components.end()&& (componentFound == nullptr)); it++) {
		if ((*it)->type == type) {
			componentFound = *it;
		}
	}
	return componentFound;
}


