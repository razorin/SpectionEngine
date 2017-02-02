#ifndef SPRIMITIVE_H
#define SPRIMITIVE_H

#include "Globals.h"

enum  SPRIMITIVE_TYPE{
		SCUBE_TYPE,
		SPLANE_TYPE,
		SCYLINDER_TYPE,
		UNDEFINED
};

class SPrimitive
{
public:
	SPrimitive();
	virtual ~SPrimitive();
public:
	float *vertices = 0;
	uint *vertexIndices = 0;
	bool toDelete = false;
};




#endif // !SPRIMITIVE_H

