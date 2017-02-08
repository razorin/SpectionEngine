#include "SCube.h"

SCube::SCube()
{

	// Create cube vertex array

	float z = -2;

	this->vertices = new float[24]{
		// ----- CUBE ----- 8verts 12tris
		//    6-------7
		//   /|      /|
		//  2-------3 |
		//  | |     | |
		//  | |4----|-|5
		//  |/      |/
		//  0-------1

		-0.5f, -0.5f,  0.5f+z,		//0
		 0.5f, -0.5f,  0.5f+z,		//1
		-0.5f,  0.5f,  0.5f+z,		//2
		 0.5f,  0.5f,  0.5f+z,		//3
		-0.5f, -0.5f,  -0.5f+z,		//4
		 0.5f, -0.5f,  -0.5f+z,		    //5
		-0.5f,  0.5f,  -0.5f+z,		//6
		 0.5f,  0.5f,  -0.5f+z		    //7		
	};
	// Create cube indices array
	this->vertexIndices = new uint[36]{
		2,0,1,		2,1,3, //Front
		3,1,5,		3,5,7, //Right
		3,7,6,		3,6,2, //Top
		6,0,2,		6,4,0, //Left
		6,5,4,		6,7,5, //Back
		4,5,1,		4,1,0 //Bottom
	};
}


SCube::~SCube()
{
}


