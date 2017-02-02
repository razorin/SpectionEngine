#ifndef MODULE_PRIMITIVES_H
#define MODULE_PRIMITIVES_H

#include "Module.h"
#include <list>
#include "SPrimitive.h"

struct json_value_t;
typedef struct json_value_t  JSON_Value;
struct json_object_t;
typedef struct json_object_t  JSON_Object;

class ModulePrimitives : public Module {
public:
	ModulePrimitives(const JSON_Value *json, bool enable);
	~ModulePrimitives();
	update_status PreUpdate();
	bool CleanUp();
	SPrimitive * AddPrimitive(const std::list<float> &points, float size, SPRIMITIVE_TYPE type);

private:
	std::list<SPrimitive*> primitives;
};


#endif // !MODULE_PRIMITIVES_H

