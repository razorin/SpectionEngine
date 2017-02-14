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
	void DrawDirectCube();

	bool Blit(SDL_Texture* texture, iPoint &position, Frame* section, bool flip = false, float speed = 1.0f);
	bool StaticBlit(SDL_Texture* texture, const iPoint &position, const SDL_Rect &section);

	bool DrawQuad(const Collider& collider, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);
	bool DrawRect(const SDL_Rect &rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

private:
	void GetHWAndDriverCapabilities();

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
	bool vsync = false;

	SDL_GLContext context;
};

#endif // __MODULERENDER_H__