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

		-0.5f, -0.5f, 0.5f,		//0
		0.5f, -0.5f, 0.5f,		//1
		-0.5f, 0.5f, 0.5f,		//2
		0.5f, 0.5f, 0.5f,		//3
		-0.5f, 0.5f, -0.5f,		//4
		0.5f, -0.5f, -0.5,		//5
		-0.5f, 0.5f, -0.5f,		//6
		0.5f, 0.5f, -0.5f		//7		
	};
	// Create cube indices array
	this->vertexIndices = new uint[36]{
		2,0,1,
		2,1,3,
		3,1,5,
		3,5,7,
		3,7,6,
		3,6,2,
		6,0,2,
		6,4,0,
		6,5,4,
		6,7,5,
		4,5,1,
		4,1,0
	};
}


SCube::~SCube()
{
}


