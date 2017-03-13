#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include<list>
#include "Module.h"
#include "MemLeaks.h"
#include "assimp\cimport.h"
#include <map>


struct SDL_Texture;

class ModuleTextures : public Module
{
private:
	struct LessString {
		bool operator()(const aiString &left, const aiString &right) const {
			return ::strcmp(left.data, right.data) < 0;
		}
	};

	typedef std::map < aiString, GLuint, LessString> TextureList;

public:
	ModuleTextures(const JSON_Object *json = nullptr);
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	uint LoadTexture(const aiString &imagePath);

private:
	TextureList textures;

};

#endif // __MODULETEXTURES_H__