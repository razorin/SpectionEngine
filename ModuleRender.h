#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "SDL\include\SDL_video.h"
#include "MemLeaks.h"

template <class TYPE> class Point;
typedef Point<int> iPoint;

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
struct Frame;
struct Collider;

class SPrimitive;
class SCube;
class SPlane;
class SCylinder;

class Light;

class ModuleRender : public Module
{
public:
	ModuleRender(const JSON_Object *json = nullptr);
	~ModuleRender();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawGrid();
	void DrawGizmo();
	void DrawDirectCube();

private:
	void GetHWAndDriverCapabilities();


public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
	bool vsync = false;

	SDL_GLContext context;


	float* positionLight = nullptr;
	float* diffuseLight = nullptr;
};

#endif // __MODULERENDER_H__