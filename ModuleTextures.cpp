#include <iostream>
#include <fstream>
#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
//DevIL
#include "DevIL\include\IL\il.h"
#include "DevIL\include\IL\ilu.h"
#include "DevIL\include\IL\ilut.h"


using namespace std;

ModuleTextures::ModuleTextures(const JSON_Object *json) : Module(json)
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{
	IMG_Quit();
}

// Called before render is available
bool ModuleTextures::Init()
{
	App->gui->console.AddLog("Init Image library");
	DLOG("Init Image library");
	bool ret = true;

	ilInit();
	ilutRenderer(ILUT_OPENGL);

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		App->gui->console.AddLog("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		DLOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	return true;
}


uint ModuleTextures::LoadTexture(const aiString &imagePath) {
	GLuint imageID = 0;
	ILboolean success;
	std::map<aiString, GLuint, LessString>::iterator it = textures.find(imagePath);

	if (it != textures.end())
		return it->second;

	success = ilLoadImage(imagePath.data);

	if (success) {
		imageID = ilutGLBindTexImage();
		textures[imagePath] = imageID;
	}

	return imageID;
}