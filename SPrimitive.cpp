#include "SPrimitive.h"



SPrimitive::SPrimitive()
{
}


SPrimitive::~SPrimitive()
{
	RELEASE(this->vertices);
	RELEASE(this->vertexIndices);
}
