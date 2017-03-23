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
	GameObject(std::string id, GameObject* parent, const char* name, bool editableName);
	GameObject(std::string id, GameObject* parent, const char* name, bool editableName, const float3& position, const float3& scale, const Quat& rotation);
	virtual ~GameObject();

	void AssignTransform();
	GameObject* GetParent() const;
	void SetParent(GameObject* parentGO);
	void OnTransformChange();

	Component * AddComponent(const ComponentType &type);
	bool RemoveComponent(Component* component);
	bool RemoveChild(GameObject* go);
	std::list<Component *> * FindComponents(const ComponentType &type);
	Component* FindComponent(const ComponentType &type);

	GameObject* FindGOByName(const std::string &name);

	void Draw() const;
	bool CleanUp();

	void AddComponentMesh(Mesh * mesh);
	void DrawBoundingBoxes() const;
	std::string GetID();
	std::string GetName();
	void SetName(std::string value, bool editable = false);
	bool IsEditableName();
	bool IsToDelete();
	void SetToDelete(bool value);
	bool IsStatic();
	void SetStatic(bool value);
	void DrawGUIPanel();


private:
	std::string id = "";
	std::string name = "";
	bool editableName = false;
	GameObject *parent = nullptr;
	bool enable = true;
	bool staticObject = false;
	std::map<ComponentType, int> componentCounterByType;
	int componentCounter = 0;
	std::list<math::AABB> AABBoxes;

	ComponentType newComponentType;

	bool toDelete = false;

public:
	ComponentTransform* transform = nullptr;
	std::list<GameObject *> childs;
	std::list<Component *> components;
};

#endif // !GAMEOBJECT_H

