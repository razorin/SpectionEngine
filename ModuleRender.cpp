#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "ModuleCollision.h"

ModuleRender::ModuleRender(const JSON_Object *json) : Module(json)
{

	vsync = json_object_dotget_boolean(json, "vsync");
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	camera.x = camera.y = 0;
	camera.w = App->window->screen_width * App->window->screen_size;
	camera.h = App->window->screen_height* App->window->screen_size;

	bool ret = true;
	Uint32 flags = 0;

	if(vsync == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	// debug camera
	int speed = 1;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->renderer->camera.y += speed;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->renderer->camera.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->renderer->camera.x += speed;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->renderer->camera.x -= speed;

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, iPoint &position, Frame* frame, bool flip, float speed)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + (position.x + (flip ? -frame->offset_x : frame->offset_x)) * App->window->screen_size;
	rect.y = (int)(camera.y * speed) + (position.y + frame->offset_y + position.z) * App->window->screen_size;

	if(frame != NULL)
	{
		rect.w = frame->section.w;
		rect.h = frame->section.h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= App->window->screen_size;
	rect.h *= App->window->screen_size;
	if (!flip) {
		if(SDL_RenderCopy(renderer, texture, &frame->section, &rect) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}
	else {
		if (SDL_RenderCopyEx(renderer, texture, &frame->section, &rect, NULL, nullptr, SDL_FLIP_HORIZONTAL) != 0) {
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}

	return ret;
}

bool ModuleRender::StaticBlit(SDL_Texture* texture, const iPoint &position, const SDL_Rect &section) {
	bool ret = true;
	SDL_Rect rec(section);

	rec.x = (int)(position.x * App->window->screen_size);
	rec.y = (int)(position.y  * App->window->screen_size);
	rec.w *= App->window->screen_size;
	rec.h *= App->window->screen_size;
	
	if (SDL_RenderCopy(renderer, texture, &section, &rec) != 0) {
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const Collider& collider, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(collider.rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + collider.rect.x * App->window->screen_size);
		rec.y = (int)(camera.y + (collider.rect.y + collider.z) * App->window->screen_size);
		rec.w *= App->window->screen_size;
		rec.h *= App->window->screen_size;
	}

	if (SDL_RenderFillRect(renderer, &rec) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawRect(const SDL_Rect &rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	rec.x = (int)(rec.x * App->window->screen_size);
	rec.y = (int)(rec.y  * App->window->screen_size);
	rec.w *= App->window->screen_size;
	rec.h *= App->window->screen_size;

	if (SDL_RenderFillRect(renderer, &rec) != 0) {
		LOG("Cannot draw rectangle to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
