#include "SPlane.h"



SPlane::SPlane()
{
	float z = -0.5f;
	// Create plane vertex array
	vertices = new float[27]{
		// ----- PLANE ----- 9verts 8tris
		//      0---1---2
		//     /   /   /
		//    3---4---5
		//   /   /   /
		//  6---7---8

		-0.5f, 0.0f, -0.5f + z,		//0
		0.0f, 0.0f, -0.5f + z,		//1
		0.5f, 0.0f, -0.5f + z,		//2
		-0.5f,0.0f, 0.0f + z,		//3
		0.0f, 0.0f, 0.0f + z,		//4
		0.5f, 0.0f, 0.0f + z,		//5
		-0.5f, 0.0f, 0.5f + z,		//6
		0.0f,0.0f, 0.5f + z,		//7
		0.5f,0.0f, 0.5f + z			//8
	};
	// Create plane indices array
	indices = new uint[24]{
		0,3,4,
		0,4,1,
		1,4,5,
		1,5,2,
		3,6,7,
		3,7,4,
		4,7,8,
		4,8,5
	};
}


SPlane::~SPlane()
{
}
