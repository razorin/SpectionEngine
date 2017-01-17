#ifndef JAKE_H
#define JAKE_H

#include "Enemy.h"

class Jake : public Enemy {
public:
	Jake(const JSON_Object *bredConfig);
	Jake(const Jake *other);
	~Jake();
	void Init(const iPoint &initialPosition);
private:
	static int number_of_instances;
};

#endif // !JAKE_H

