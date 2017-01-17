#ifndef BRED_H
#define BRED_H

#include "Enemy.h"

class Bred : public Enemy {
public:
	Bred(const JSON_Object *bredConfig);
	Bred(const Bred *other);
	~Bred();
	void Init(const iPoint &initialPosition);
private:
	static int number_of_instances;

};

#endif // !BRED_H
