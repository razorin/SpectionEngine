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
	GameObject(std::string id, GameObject* parent, const char* name);
	GameObject(std::string id, GameObject* parent, const char* name, const float3& position, const float3& scale, const Quat& rotation);
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
	void DrawGUIPanel();


private:
	std::string id = 0;
	GameObject *parent = nullptr;
	bool enable = true;
	std::map<ComponentType, int> componentCounterByType;
	int componentCounter = 0;
	std::list<math::AABB> AABBoxes;

	ComponentType newComponentType;

public:
	std::string name = "";
	ComponentTransform* transform = nullptr;
	std::list<GameObject *> childs;
	std::list<Component *> components;
};

#endif // !GAMEOBJECT_H

