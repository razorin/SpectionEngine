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
	ComponentMesh(GameObject* container, std::string id);
	~ComponentMesh();

	bool DrawGUI();

public:
	Mesh* mesh = nullptr;
	//std::vector<unsigned> meshes;

};

#endif