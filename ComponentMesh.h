#ifndef COMPONENTMESH_H
#define COMPONENTMESH_H

#include "Globals.h"
#include "Component.h"
#include "Mesh.h"

class Scene;

class ComponentMesh :
	public Component
{
public:
	ComponentMesh(GameObject* container);
	~ComponentMesh();

public:
	//TODO use this reference of a scene
	Scene* scene = nullptr;
	
	std::vector<unsigned> meshes;

};

#endif