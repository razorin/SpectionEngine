#include "Mesh.h"
#include "assimp\vector3.h"
#include "Glew/include/GL/glew.h"


// Mesh
Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	RELEASE(this->normals);
	RELEASE(this->colors);
	RELEASE(this->indices);
	RELEASE(this->vertices);
	RELEASE_ARRAY(this->textureCoords);
	//RELEASE(vboTextures);

	for (int i = 0; i < numBones; i++)
	{
		RELEASE_ARRAY(this->bones[i].weights);
	}
	RELEASE_ARRAY(this->bones);
}

void Mesh::InitializeBuffers()
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
		//vboTextures = new uint[numTextures];
		//for (int i = 0; i < numTextures; i++) {
		//	glGenBuffers(1, (GLuint*)&(vboTextures[i]));
		//	glBindBuffer(GL_ARRAY_BUFFER, vboTextures[i]);
		//	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* numVertices * 3, textureCoords[i], GL_STATIC_DRAW);
		//}
		glGenBuffers(1, (GLuint*)&(vboTextures));
		glBindBuffer(GL_ARRAY_BUFFER, vboTextures);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* numVertices * 3, textureCoords, GL_STATIC_DRAW);
	}

	//Multiple textures
	/*if (textureCoords != nullptr) {
		for (int i = 0; i < 8; i++) {
			if (textureCoords[i] != nullptr)
			{
				DLOG("textureCoords: %f   %f", textureCoords[0][0], textureCoords[0][1]);
				glGenBuffers(1, (GLuint*)&(vbo_textures));
				glBindBuffer(GL_ARRAY_BUFFER, vbo_textures);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)* numVertices * 2, textureCoords[i], GL_STATIC_DRAW);
			}
		}
	}*/
	
}

void Mesh::Draw() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	

	if (normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
		glNormalPointer(GL_FLOAT, sizeof(float)*3, 0);
	}

	if (colors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vboColors);
		glColorPointer(3, GL_FLOAT, 0, NULL);
	}

	if (textureCoords != nullptr)
	{
		//for (int i = 0; i < numTextures; i++) {
		//	glBindTexture(GL_TEXTURE_2D, imageName);
		//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//	glBindBuffer(GL_ARRAY_BUFFER, vboTextures[i]);
		//	//Deberia estar fuera
		//	//glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		//}
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		glBindTexture(GL_TEXTURE_2D, imageName);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vboTextures);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);


	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
