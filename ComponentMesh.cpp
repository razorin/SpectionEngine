#include "ComponentMesh.h"
#include "assimp\vector3.h"
#include "Glew/include/GL/glew.h"

ComponentMesh::ComponentMesh(GameObject * container) :Component(container, ComponentType::COMPONENT_TYPE_MESH)
{
}

ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::DrawGUI()
{
	return false;
}