#ifndef SIMONS_H
#define SIMONS_H

#include "Enemy.h"


class Simons : public Enemy {
public:
	Simons(const JSON_Object *bredConfig);
	Simons(const Simons *other);
	~Simons();
	void Init(const iPoint &initialPosition);
private:
	static int number_of_instances;
};

#endif // !SIMONS_H


