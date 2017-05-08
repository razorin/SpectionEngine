#ifndef __MODULEPROGRAM_H__
#define __MODULEPROGRAM_H__

#include "Globals.h"
#include "assimp\cimport.h"
#include <map>
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


class ProgramManager
{
public:

	struct LessString {
		bool operator()(const aiString& left, const aiString& right) {
			return ::strcmp(left.data, right.data) < 0;
		}
	};

	typedef std::map<aiString, LessString> ProgramList;

	ProgramList programs;
	static ProgramManager* instance;

private:
	ProgramManager();

public:
	~ProgramManager();

	void Load(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath);
	void Clear();

	int GetUniformLocation(const char* name, const char* uniform);
	void UseProgram(const char* name);
	void UnuseProgram();

	std::string ReadShader(const char *filePath);

	static ProgramManager* GetInstance();
};

#endif