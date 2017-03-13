#ifndef MODULE_PRIMITIVES_H
#define MODULE_PRIMITIVES_H

#include "Module.h"
#include <list>
#include "SPrimitive.h"
#include "MemLeaks.h"
#include "Point.h"


struct json_value_t;
typedef struct json_value_t  JSON_Value;
struct json_object_t;
typedef struct json_object_t  JSON_Object;

class ModulePrimitives : public Module {
public:
	ModulePrimitives(const JSON_Value *json = nullptr, bool enable = true);
	~ModulePrimitives();
	update_status PreUpdate();
	bool CleanUp();
	SPrimitive * AddPrimitive(SPRIMITIVE_TYPE type, const fPoint &position = { 0,0,0 }, float scale = 1.f);

private:
	std::list<SPrimitive*> primitives;
};


#endif // !MODULE_PRIMITIVES_H

