#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <map>
#include <list>
#include "MathGeoLib\include\MathGeoLib.h"

class Component;
class ComponentTransform;
class Mesh;
enum ComponentType;

class GameObject
{
public:
	GameObject(GameObject* parent = nullptr, const char* name = "EmptyGO");
	GameObject(GameObject* parent, const char* name, const float3& position, const float3& scale, const Quat& rotation);
	virtual ~GameObject();

	void AssignTransform();
	GameObject* GetParent() const;
	void SetParent(GameObject* parentGO);
	void OnTransformChange();

	Component * AddComponent(const ComponentType &type);
	bool RemoveComponent(Component *);
	std::list<Component *> * FindComponents(const ComponentType &type);
	Component* FindComponent(const ComponentType &type);

	void Draw() const;
	bool CleanUp();

	void AddComponentMesh(Mesh *);
	void DrawBoundingBoxes() const;


private:
	GameObject *parent = nullptr;
	bool enable = true;
	std::map<ComponentType, int> componentCounter;
	std::list<math::AABB> AABBoxes;

public:
	std::string name = "";
	ComponentTransform* transform = nullptr;
	std::list<GameObject *> childs;
	std::list<Component *> components;
};

#endif // !GAMEOBJECT_H

