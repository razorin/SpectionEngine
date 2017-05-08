#include "ProgramManager.h"



ProgramManager::ProgramManager()
{
}


ProgramManager::~ProgramManager()
{
}

void ProgramManager::Load(const char * name, const char * vertexShaderPath, const char * fragmentShaderPath)
{
	//Load Shaders
	char* vertexShader = ReadShader(vertexShaderPath);
	char* fragmentShader = ReadShader(fragmentShaderPath);
	//Create Shaders
	uint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	uint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	//Create Program
	uint id = glCreateProgram();
	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);
	glLinkProgram(id);
	glUseProgram(id);

}

void ProgramManager::Clear()
{
}

int ProgramManager::GetUniformLocation(const char * name, const char * uniform)
{
	return 0;
}

void ProgramManager::UseProgram(const char * name)
{
}

void ProgramManager::UnuseProgram()
{
}

char * ProgramManager::ReadShader(const char * name)
{
	return nullptr;
}

ProgramManager * ProgramManager::GetInstance()
{
	return nullptr;
}
