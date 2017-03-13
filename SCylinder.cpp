#include "SCylinder.h"
#include <cmath>


SCylinder::SCylinder(const fPoint &position, float scale) : SCylinder(position, scale, 6)
{

}

SCylinder::SCylinder(const fPoint &position, float scale, int numberOfFaces) {
	numVertices = 12;
	float calculatedCos = cos((360 / numberOfFaces) * DEGTORAD);
	float calculatedSen = sin((360.0f / numberOfFaces) * DEGTORAD);
	float height = scale;
	float width = scale / 2;

	// Create cylinder vertex array
	vertices = new float[numVertices * 3]{
		// ----- CYLINDER ----- 12verts 20tris
		//      0---1
		//     /|   |\
		//    5 |   | 2
		//    |\|   |/|
		//    | 4---3 |
		//    | |   | |
		//    | 6---7 |
		//    |/|   |\|
		//    11|   | 8
		//     \|   |/
		//     10---9
		
		position.x - width * calculatedCos, position.y + height, position.z - width * calculatedSen,			//0
		position.x + width * calculatedCos, position.y + height, position.z - width * calculatedSen,			//1
		position.x + width, position.y + height, position.z,														//2
		position.x + width * calculatedCos, position.y + height, position.z + width * calculatedSen,			//3

		position.x - width * calculatedCos, position.y + height, position.z + width * calculatedSen,			//4
		position.x - width, position.y + height, position.z,														//5
		position.x - width * calculatedCos, position.y - height, position.z - width * calculatedSen,		//6
		position.x + width * calculatedCos, position.y - height, position.z - width * calculatedSen,			//7
		position.x + width, position.y - height, position.z,														//8
		position.x + width * calculatedCos, position.y - height, position.z + width * calculatedSen,			//9
		position.x - width * calculatedCos, position.y - height, position.z + width * calculatedSen,			//10
		position.x - width, position.y - height, position.z

	};

	numIndices = 60;
	// Create cylinder indices array
	indices = new uint[60]{
		// Top face
		0,2,1,
		0,5,4,
		0,4,3,
		0,3,2,
		// Bottom face
		6,7,8,
		6,8,9,
		6,9,10,
		6,10,11,
		// Sides
		5,11,10,
		5,10,4,
		4,10,9,
		4,9,3,
		3,9,8,
		3,8,2,
		2,8,7,
		2,7,1,
		1,7,6,
		1,6,0,
		0,6,11,
		0,11,5
	};

	SPrimitive::InitializeBuffers();
}


SCylinder::~SCylinder()
{
}
