#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"
#include "MemLeaks.h"

struct SDL_Window;
struct SDL_Surface;

enum DisplayMode {
	FULLSCREEN,
	BORDERLESS,
	FULLSCREENWINDOWED
};

class ModuleWindow : public Module
{
public:

	ModuleWindow(const JSON_Object *json = nullptr);
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void ChangeTitle(const char* title);
	void SetResizable(bool value);
	void SetDisplayMode(DisplayMode value);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	int screen_width = 0;
	int screen_height = 0;
	bool fullscreen = false;
	bool borderless = false;
	bool resizable = false;
	bool fullscreenWindowed = false;
	int screen_size = 0;
	const char *title;
};

#endif // __MODULEWINDOW_H__