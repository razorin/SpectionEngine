#ifndef SCYLINDER_H
#define SCYLINDER_H

#include "SPrimitive.h"
#include "Point.h"

class SCylinder :
	public SPrimitive
{
public:
	SCylinder(const fPoint &position = fPoint{ 0,0,0 }, float scale = 1);
	~SCylinder();
private:
	/**TODO: Finish to refactor indexes and vertex so we can use @numberOfFace parameter in public
	access constructor instead of private one
	**/
	SCylinder(const fPoint &position, float scale, int numberOfFaces);
};

#endif // !SCYLINDER_H
