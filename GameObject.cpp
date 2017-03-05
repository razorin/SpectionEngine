#include "GameObject.h"
#include "Component.h"
#include "Globals.h"
#include "ComponentCamera.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentModel.h"
#include "ComponentScript.h"
#include "ComponentTransform.h"

GameObject::GameObject()
{
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

Component * GameObject::addComponent(const ComponentType &type)
{
	Component *result = nullptr;
	//TODO: Create component
	//TODO: Review max number of components allowed by GameObject
	//TODO: Increment componentCounter!!!
	switch (type) {
	case ComponentType::COMPONENT_TYPE_CAMERA:
		//Create component
		result = new ComponentCamera();
		//Review max number of components allowed by GameObject
		if (componentCounter[type] < result->maxNumberOfComponentByGameObject) {
			//Set type
			result->type = type;
			//Add to the components list
			components.push_back(result);
			//Increment componentCounter!!!
			componentCounter[type] = componentCounter[type] + 1;
		}
		else {
			RELEASE(result);
		}
		break;
	case ComponentType::COMPONENT_TYPE_LIGHT:
		//Create component
		result = new ComponentLight();
		//Review max number of components allowed by GameObject
		if (componentCounter[type] < result->maxNumberOfComponentByGameObject) {
			//Set type
			result->type = type;
			//Add to the components list
			components.push_back(result);
			//Increment componentCounter!!!
			componentCounter[type] = componentCounter[type] + 1;
		}
		else {
			RELEASE(result);
		}
		break;
	case ComponentType::COMPONENT_TYPE_MATERIAL:
		//Create component
		result = new ComponentMaterial();
		//Review max number of components allowed by GameObject
		if (componentCounter[type] < result->maxNumberOfComponentByGameObject) {
			//Set type
			result->type = type;
			//Add to the components list
			components.push_back(result);
			//Increment componentCounter!!!
			componentCounter[type] = componentCounter[type] + 1;
		}
		else {
			RELEASE(result);
		}
		break;
	case ComponentType::COMPONENT_TYPE_MODEL:
		//Create component
		result = new ComponentModel();
		//Review max number of components allowed by GameObject
		if (componentCounter[type] < result->maxNumberOfComponentByGameObject) {
			//Set type
			result->type = type;
			//Add to the components list
			components.push_back(result);
			//Increment componentCounter!!!
			componentCounter[type] = componentCounter[type] + 1;
		}
		else {
			RELEASE(result);
		}
		break;
	case ComponentType::COMPONENT_TYPE_SCRIPT:
		//Create component
		result = new ComponentScript();
		//Review max number of components allowed by GameObject
		if (componentCounter[type] < result->maxNumberOfComponentByGameObject) {
			//Set type
			result->type = type;
			//Add to the components list
			components.push_back(result);
			//Increment componentCounter!!!
			componentCounter[type] = componentCounter[type] + 1;
		}
		else {
			RELEASE(result);
		}
		break;
	case ComponentType::COMPONENT_TYPE_TRANSFORM:
		//Create component
		result = new ComponentTransform();
		//Review max number of components allowed by GameObject
		if (componentCounter[type] < result->maxNumberOfComponentByGameObject) {
			//Set type
			result->type = type;
			//Add to the components list
			components.push_back(result);
			//Increment componentCounter!!!
			componentCounter[type] = componentCounter[type] + 1;
		}
		else {
			RELEASE(result);
		}
		break;
	}

	return result;
}

void GameObject::removeComponent(Component *component)
{
	bool found = false;
	//Find Component
	for (std::list<Component *>::iterator it = components.begin(); it != components.end();) {
		if (component == (*it)) {
			ComponentType type = (*it)->type;
			//Remove and release element
			delete (*it);
			it = components.erase(it);
			//Decrement componentCounter (assuming we can only have 1 component of each type per GameObject)
			componentCounter[type]--;
			//Found
			found = true;
		}
		
		++it;
	}
	
	if (!found) DLOG("Component not found!");
	
}

void GameObject::Update()
{
}

