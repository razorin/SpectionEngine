#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "SDL/include/SDL.h"
#include "SDL\include\SDL_video.h"
#include "Parson.h"

ModuleWindow::ModuleWindow(const JSON_Object *json) : Module(json)
{

	screen_width = (int)json_object_dotget_number(json, "resolution.width");
	screen_height = (int)json_object_dotget_number(json, "resolution.height");;
	fullscreen = json_object_dotget_boolean(json, "fullscreen");
	borderless = (bool)json_object_dotget_boolean(json, "borderless");
	resizable = json_object_dotget_boolean(json, "resizable");
	fullscreenWindowed = json_object_dotget_boolean(json, "fullscreenWindowed");
	screen_size = (int)json_object_dotget_number(json, "size");
	title = json_object_dotget_string(json, "title");
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	App->gui->console.AddLog("Init SDL window & surface");
	DLOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		DLOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = screen_width * screen_size;
		int height = screen_height * screen_size;
		Uint32 flags = SDL_WINDOW_SHOWN;

		if (fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		//Add flag for OpenGL
		flags |= SDL_WINDOW_OPENGL;

		if (resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		//Set openGL render driver
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == nullptr)
		{
			DLOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

void ModuleWindow::ChangeTitle(const char* title)
{
	this->title = title;
	SDL_SetWindowTitle(window, this->title);
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	DLOG("Destroying SDL window and quitting all SDL systems");
	SDL_SetWindowBrightness(window, 1.0f);
	//Destroy window
	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetResizable(bool value) {
	this->resizable = value;
	// TODO: Either upgrade to SDL 2.0.5 or change the configuration json and ask to restart to apply it
	if (resizable) {
		//SDL_SetWindowResizable(window, true); //------------------- IN SDL 2.0.5
	}
	else {
		//SDL_SetWindowResizable(window, false); //------------------- IN SDL 2.0.5
	}
}

void ModuleWindow::SetDisplayMode(DisplayMode value) {
	switch (value) {
		// TODO: Each case should change the configuration json to save the current settings.
	case FULLSCREEN:
		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0) {
			App->gui->console.AddLog("Error on switching to fullscreen: %s", SDL_GetError());
		}
		break;
	case BORDERLESS:
		if (SDL_SetWindowFullscreen(window, NULL) != 0) {
			App->gui->console.AddLog("Error on switching to windowed borderless: %s", SDL_GetError());
		}
		// TODO: Fix bordered, if it's used without previously changing to another display mode it freezes the screen.
		//SDL_SetWindowBordered(window, SDL_FALSE);
		break;
	case FULLSCREENWINDOWED:
		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
			App->gui->console.AddLog("Error on switching to fullscreen windowed: %s", SDL_GetError());
		}
		break;
	}
}

void ModuleWindow::SetBrightness(int value) {
	float brightness = (float)value / 1000.0f;
	if (brightness <= 0.228f) {
		brightness = 0.228f;
	}
	if (SDL_SetWindowBrightness(window, brightness) != 0) {
		App->gui->console.AddLog("Error on changing window brightness: %s", SDL_GetError());
	}
	if (brightness = 1.0f) {
		int a = 0;
	}
}