#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "Parson.h"

ModuleWindow::ModuleWindow(const JSON_Object *json) : Module(json)
{

	screen_width = (int)json_object_dotget_number(json, "resolution.width");
	screen_height = (int)json_object_dotget_number(json, "resolution.height");;
	fullscreen = json_object_dotget_boolean(json, "fullscreen");
	borderless = (bool)json_object_dotget_boolean(json, "borderless");
	resizable = json_object_dotget_boolean(json, "resizable");
	fullscreen_window = json_object_dotget_boolean(json, "fullscren_window");
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

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
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

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		//Add flag for OpenGL
		flags |= SDL_WINDOW_OPENGL;
		//Add flag to make the window resizable
		flags |= SDL_WINDOW_RESIZABLE;

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == nullptr)
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

	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}
	
	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

