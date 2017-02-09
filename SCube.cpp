#include "SCube.h"

SCube::SCube()
{

	// Create cube vertex array
	this->vertices = new float[24]{
		// ----- CUBE ----- 8verts 12tris
		//    6-------7
		//   /|      /|
		//  2-------3 |
		//  | |     | |
		//  | |4----|-|5
		//  |/      |/
		//  0-------1

		-0.5f, -0.5f,  0.5f,		//0
		 0.5f, -0.5f,  0.5f,		//1
		-0.5f,  0.5f,  0.5f,		//2
		 0.5f,  0.5f,  0.5f,		//3
		-0.5f, -0.5f, -0.5f,		//4
		 0.5f, -0.5f, -0.5f,		//5
		-0.5f,  0.5f, -0.5f,		//6
		 0.5f,  0.5f, -0.5f			//7		
	};
	// Create cube indices array
	this->vertexIndices = new uint[36]{
		0,1,2,		2,1,3, //Front
		1,5,3,		3,5,7, //Right
		4,0,6,		6,0,2, //Left
		5,4,7,		7,4,6, //Back
		2,3,6,		6,3,7, //Top
		4,5,0,		0,5,1 //Bottom
	};
	// Create cube texture coordinates
	this->textureCoords = new float[8]{
		0,0,	//0
		0,1,	//1
		1,0,	//2
		1,1		//3
	};

	// Create cube texture indices
	this->textureIndices = new float[36]{
		0,2,1,		1,2,3, //Front
		0,2,1,		1,2,3, //Right
		0,2,1,		1,2,3, //Left
		0,2,1,		1,2,3, //Back
		0,2,1,		1,2,3, //Top
		0,2,1,		1,2,3 //Bottom
	};
}


SCube::~SCube()
{
}


