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

	struct Program {
		GLuint id;
		bool active = false;
	};

	typedef std::map<std::string, Program> ProgramList;

	ProgramList programs;
	static ProgramManager* instance;

private:
	ProgramManager();

public:
	~ProgramManager();

	void Load(const std::string name, const std::string vertexShaderPath, const std::string fragmentShaderPath);
	void Clear();

	int GetUniformLocation(const char* name, const char* uniform);
	void UseProgram(const std::string name);
	void UnuseProgram();

	std::string ReadShader(const std::string filePath);
	void SetProgramsToFalse();

	static ProgramManager* GetInstance();
};

#endif