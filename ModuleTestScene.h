#ifndef ModuleTestScene_H
#define ModuleTestScene_H

#include "Module.h"
#include "Point.h"

struct SDL_Texture;
struct Frame;


class ModuleTestScene : public Module {
public:
	ModuleTestScene(const JSON_Value *json, bool active);
	ModuleTestScene(const bool active);
	~ModuleTestScene();

	bool Start();
	update_status PreUpdate();
	update_status Update(float dt = 0);
	update_status PostUpdate();

	bool CleanUp();

public:
	SDL_Texture* graphics;
	Frame *frame = nullptr;
	iPoint position;
};

#endif // !ModuleTestScene_H