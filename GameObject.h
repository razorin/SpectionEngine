#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <map>
#include <list>
#include "MathGeoLib\include\MathGeoLib.h"

class Component;
class ComponentTransform;
enum ComponentType;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent = nullptr, const char* name = "EmptyGO");
	GameObject(GameObject* parent, const char* name, const float3& position, const float3& scale, const Quat& rotation);
	virtual ~GameObject();

	GameObject* GetParent() const;
	void SetParent(GameObject* parentGO);
	ComponentTransform* Transform();

	Component * AddComponent(const ComponentType &type);
	bool RemoveComponent(Component *);
	std::list<Component *> * FindComponents(const ComponentType &type);
	Component* FindComponent(const ComponentType &type);

	//Todo delete uptade. makes no sense in gameobject
private:
	void Update();


private:
	GameObject *parent = nullptr;
	bool enable = true;
	std::map<ComponentType, int> componentCounter;
	std::list<GameObject *> childrens;
	std::list<Component *> components;


public:
	std::string name = "";
};

#endif // !GAMEOBJECT_H

