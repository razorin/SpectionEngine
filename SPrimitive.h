#ifndef SPRIMITIVE_H
#define SPRIMITIVE_H

#include "Globals.h"

class SPrimitive
{
public:
	SPrimitive();
	virtual ~SPrimitive();
public:
	float *vertices;
	uint *vertexIndices;
};




#endif // !SPRIMITIVE_H

