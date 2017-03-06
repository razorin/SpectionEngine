#include "ComponentMesh.h"
#include "assimp\vector3.h"
#include "Glew/include/GL/glew.h"

ComponentMesh::ComponentMesh(GameObject * container) :Component(container, ComponentType::COMPONENT_TYPE_MESH)
{
}

ComponentMesh::~ComponentMesh()
{
	RELEASE(this->normals);
	RELEASE(this->colors);
	RELEASE(this->indices);
	RELEASE(this->vertices);
	RELEASE_ARRAY(this->textureCoords);
}

void ComponentMesh::InitializeBuffers()
{
	glGenBuffers(1, (GLuint*) &(vboVertices));
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(vboIndices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* numIndices, indices, GL_STATIC_DRAW);

	if (normals != nullptr)
	{
		glGenBuffers(1, (GLuint*)&(vboNormals));
		glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, normals, GL_STATIC_DRAW);
	}

	if (colors != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(vboColors));
		glBindBuffer(GL_ARRAY_BUFFER, vboColors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* numVertices * 3, colors, GL_STATIC_DRAW);
	}

	if (textureCoords != nullptr)
	{
		glGenBuffers(1, (GLuint*)&(vboTextures));
		glBindBuffer(GL_ARRAY_BUFFER, vboTextures);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* numVertices * 3, textureCoords, GL_STATIC_DRAW);
	}
}
