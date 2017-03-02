#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <map>
#include <list>

class Component;
enum ComponentType;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	Component * addComponent(const ComponentType &type);
	void removeComponent(Component *);

private:
	GameObject *parent = nullptr;
	bool enable = true;
	std::map<ComponentType, int> componentCounter;
	void Update();
	std::list<GameObject *> childrens;
	std::list<Component *> components;


public:
	std::string name = "";
};

#endif // !GAMEOBJECT_H

