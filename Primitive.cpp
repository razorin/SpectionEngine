#include "Primitive.h"



Primitive::Primitive()
{
}


Primitive::~Primitive()
{
	RELEASE(vertices);
	RELEASE(vertexIndices);
}
