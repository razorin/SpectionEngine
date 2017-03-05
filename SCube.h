#ifndef SCUBE_H
#define SCUBE_H

#include "SPrimitive.h"
#include "Point.h"


#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

class SCube : public SPrimitive {
public:
	SCube(const fPoint &position = fPoint{ 0,0,0 }, float scale = 1);
	~SCube();

public:
};
#endif // !SCUBE_H

