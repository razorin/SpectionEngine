#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include<list>
#include "Module.h"
#include "MemLeaks.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures(const JSON_Object *json = nullptr);
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);
	void LoadTexture(GLuint *texture, char imageName[100], bool alpha);
	void Unload(SDL_Texture* texture);

private:
	std::list<SDL_Texture*> textures;
};

#endif // __MODULETEXTURES_H__