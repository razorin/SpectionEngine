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

unsigned ModuleTextures::LoadTexture(const aiString& file)
{
	TextureList::iterator it = textures.find(file);

	if (it != textures.end())
	{
		return it->second;
	}

	return textures[file] = ForceLoad(file);
}

void ModuleTextures::Clear()
{
	for (TextureList::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		glDeleteTextures(1, &it->second);
	}

	textures.clear();
}

unsigned ModuleTextures::ForceLoad(const aiString& file)
{
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (ilLoadImage(file.data))
	{
		GLuint textureId = 0;
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		int channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (channels == 3) {
			ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		}
		else if (channels == 4) {
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		}

		ILubyte* data = ilGetData();
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width,
			height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, data);

		ilDeleteImages(1, &imageId);

		glBindTexture(GL_TEXTURE_2D, 0);

		return textureId;
	}

	return 0;
}