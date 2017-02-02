#pragma once
#include "Globals.h"

class Primitive
{
public:
	Primitive();
	virtual ~Primitive();
public:
	float *vertices;
	uint *vertexIndices;
};

