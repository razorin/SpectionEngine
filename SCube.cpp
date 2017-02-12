#include "SCube.h"
#include "Glew/include/GL/glew.h"
#include "Application.h"
#include "ModuleTextures.h"


SCube::SCube()
{
	float min = -0.5f;
	float max = 0.5f;

	numVertices = 12;
	vertices = new float[numVertices * 3]{
		// ----- CUBE ----- 8verts 12tris
		//    6-------7			   10-------11		
		//   /|      /|            /|      /|
		//  2-------3 |			  8-------9 |
		//  | |     | |			  | |     | |	
		//  | |4----|-|5          | |4----|-|5
		//  |/      |/            |/      |/
		//  0-------1             0-------1

		min, min, max,
		max, min, max,
		min, max, max,
		max, max, max,
		min, min, min,
		max, min, min,
		min, max, min,
		max, max, min,

		//Extra vertices, we are repeating topface vertices (2,3,6,7)
		min, max, max,
		max, max, max,
		min, max, min,
		max, max, min,
	};

	numIndices = 36;
	indices = new uint[36]{
		0,1,2,		2,1,3,  //front
		1,5,9,		9,5,11,  //right
		4,0,10,		10,0,8,  //left
		5,4,7,		7,4,6,  //back
		2,3,6,		6,3,7,  //top
		4,5,0,		0,5,1   //bottom
	};
	
	//colors = new float[numVertices * 3]{
	//	1, 1, 0,   1, 1, 0,   1, 0, 0,	 1, 0, 0,
	//	1, 1, 0,   1, 1, 0,	  1, 0, 0,   1, 0, 0
	//};



	// Load checkImage texture
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	DLOG("HOLA!!");
	//GLuint textureHandle = App->textures->LoadTexture("Lenna.png", true);

	// Load texture coord buffer
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
	//	0, GL_RGBA, GL_UNSIGNED_BYTE, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, 0);


	textureCoords = new float[numVertices * 2]{
		0,0,
		0,1,
		1,0,
		1,1,
		0,1,
		1,1,
		0,0,
		1,0,
		0,1,
		0,0,
		1,1,
		1,0,
	};


	SPrimitive::InitializeBuffers();
}


SCube::~SCube()
{
}


