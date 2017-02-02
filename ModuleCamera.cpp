#include "ModuleCamera.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL.h"
#include "Application.h"
#include "ModuleWindow.h"


ModuleCamera::ModuleCamera(const JSON_Object *json) : Module(json)
{
}


ModuleCamera::~ModuleCamera()
{
}

update_status ModuleCamera::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::Init()
{
	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::ChangeWindowSize(int width, int height)
{
	App->window->screen_width = width;
	App->window->screen_height = height;
	DLOG("HABEMUS CAMBIO!");
	DLOG("LA NUEVA WIDTH ES: %d", App->window->screen_width);
	DLOG("LA NUEVA HEIGHT ES: %d", App->window->screen_height);
}

