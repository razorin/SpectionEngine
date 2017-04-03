#include "Mesh.h"
#include "assimp\vector3.h"
#include "Glew/include/GL/glew.h"


// Mesh
Mesh::Mesh()
{
}

Mesh::Mesh(Mesh * mesh) : vboVertices(mesh->vboVertices), vboIndices(mesh->vboIndices), vboColors(mesh->vboColors),
vboNormals(mesh->vboNormals), vboTextures(mesh->vboTextures), numVertices(mesh->numVertices), numIndices(mesh->numIndices),
numTextures(mesh->numTextures), imageName(mesh->imageName), numBones(mesh->numBones)
{
	if (indices != nullptr) {
		indices = new uint[numIndices];
		std::copy(mesh->indices, mesh->indices + numIndices, indices);
	}
	if (vertices != nullptr) {
		vertices = new float[numVertices];
		std::copy(mesh->vertices, mesh->vertices + numVertices, vertices);
	}
	if (colors != nullptr) {
		colors = new float[numIndices];
		std::copy(mesh->colors, mesh->colors + numIndices, colors);
	}
	if (normals != nullptr) {
		normals = new float[numIndices];
		std::copy(mesh->normals, mesh->normals + numIndices, normals);
	}
	if (textureCoords != nullptr) {
		textureCoords = new float[numTextures];
		std::copy(mesh->textureCoords, mesh->textureCoords + numTextures, textureCoords);
	}
	if (bones != nullptr) {
		bones = new Bone[numBones];
		for (int i = 0; i < numBones; ++i) {
			bones[i] = new Bone(mesh->bones[i]);
		}
	}
}


Mesh::~Mesh()
{
	RELEASE(this->normals);
	//RELEASE(this->colors);
	RELEASE(this->indices);
	RELEASE(this->vertices);
	RELEASE_ARRAY(this->textureCoords);

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
		glNormalPointer(GL_FLOAT, sizeof(float) * 3, 0);
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

void Mesh::Deform()
{
	if (numBones <= 0)
		return;

	float *newVertices = new float[numVertices*3];
	memset(newVertices, 0, 3* numVertices * sizeof(float));


	//memset(vertices, 0, numVertices * sizeof(float3));
	float4x4 influenceMatrix = float4x4::identity;

	for (int i = 0; i < numBones; i++)
	{
		influenceMatrix = bones[i].ownerGOTransform->GlobalTransform() * bones[i].bind;

		for (int j = 0; j < bones[i].numWeights; j++)
		{
			uint vertexIndex = bones[i].weights[j].vertex * 3;
			float3 originalVertex = float3{ vertices[vertexIndex], vertices[vertexIndex + 1], vertices[vertexIndex + 2] };
			float3 vertexOffset = 100 * bones[i].weights[j].weight * influenceMatrix.TransformPos(originalVertex);

			newVertices[vertexIndex] += vertexOffset.x;
			newVertices[vertexIndex + 1] += vertexOffset.y;
			newVertices[vertexIndex + 2] += vertexOffset.z;
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 3, newVertices, GL_STATIC_DRAW);

	delete(newVertices);
}
