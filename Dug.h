#ifndef DUG_H
#define DUG_H

#include "Enemy.h"

class Dug : public Enemy {
public:
	Dug(const JSON_Object *bredConfig);
	Dug(const Dug *other);
	~Dug();
	void Init(const iPoint &initialPosition);
private:
	static int number_of_instances;
};

#endif // !DUG_H

