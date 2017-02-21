#include "Mesh.h"
#include "assimp\vector3.h"
#include "Glew/include/GL/glew.h"

// Mesh
Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	RELEASE(this->textureCoords);
	RELEASE(this->normals);
	RELEASE(this->colors);
	RELEASE(this->indices);
	RELEASE(this->vertices);
}

void Mesh::InitializeBuffers()
{
	glGenBuffers(1, (GLuint*) &(vbo_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(vbo_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* numIndices, indices, GL_STATIC_DRAW);

	if (colors != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(vbo_colors));
		glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* numVertices * 3, colors, GL_STATIC_DRAW);
	}

	if (textureCoords != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(vbo_textures));
		glBindBuffer(GL_ARRAY_BUFFER, vbo_textures);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* numVertices * 2, textureCoords, GL_STATIC_DRAW);
	}
}

void Mesh::Draw() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (colors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
		glColorPointer(3, GL_FLOAT, 0, NULL);
	}

	if (textureCoords != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, ImageName);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_textures);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);


	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
