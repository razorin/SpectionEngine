#ifndef ModuleTestScene_H
#define ModuleTestScene_H

#include <list>
#include "Module.h"
#include "SPrimitive.h"
#include "MemLeaks.h"

class SPlane;
class SCube;
class SCylinder;
class Model;


class ModuleTestScene : public Module {
public:
	ModuleTestScene(const JSON_Value *json, bool active);
	ModuleTestScene(const bool active);
	~ModuleTestScene();

	bool Start();
	update_status PreUpdate();
	update_status Update(float dt = 0);
	update_status PostUpdate();

	void Draw();

	bool CleanUp();

public:
	SCube *cube = nullptr;
	SPlane *plane = nullptr;
	SCylinder *cylinder = nullptr;
	Model *model = nullptr;
	Model *model2 = nullptr;

private:
	std::list<SPrimitive*> primitives;

};

#endif // !ModuleTestScene_H