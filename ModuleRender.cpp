#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "Animation.h"
#include "ModuleCollision.h"
#include "Parson.h"
#include "SDL/include/SDL.h"
#include "Glew/include/GL/glew.h"

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
	DLOG("Creating Renderer context");

	//OpenGL Inicialization
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

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
	
	//Create OpenGL Context
	context = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();
	//Checking errors
	if (GLEW_OK != err)
	{
		// glewInit failed
		DLOG("Error on glewInit: %s", glewGetErrorString(err));
	}
	// Should be 2.0
	DLOG("Using Glew %s", glewGetString(GLEW_VERSION));
	
	GetHWAndDriverCapabilities();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	
	if(renderer == nullptr)
	{
		DLOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate(float dt)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update(float dt)
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

update_status ModuleRender::PostUpdate(float dt)
{
	SDL_RenderPresent(renderer);

	//Swap Buffer (OpenGL)
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	DLOG("Destroying renderer");

	SDL_GL_DeleteContext(context);

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
			DLOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}
	else {
		if (SDL_RenderCopyEx(renderer, texture, &frame->section, &rect, NULL, nullptr, SDL_FLIP_HORIZONTAL) != 0) {
			DLOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
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
		DLOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
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
		DLOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
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
		DLOG("Cannot draw rectangle to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void ModuleRender::GetHWAndDriverCapabilities()
{
	DLOG("Vendor: %s", glGetString(GL_VENDOR));
	DLOG("Renderer: %s", glGetString(GL_RENDERER));
	DLOG("OpenGL version supported %s", glGetString(GL_VERSION));
	DLOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}
