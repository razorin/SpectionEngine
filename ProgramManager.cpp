#include "ProgramManager.h"
#include <string>
#include <iostream>
#include <fstream>

//http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/

ProgramManager::ProgramManager()
{
}


ProgramManager::~ProgramManager()
{
}

void ProgramManager::Load(const char * name, const char * vertexShaderPath, const char * fragmentShaderPath)
{
	//Read Shaders
	/*std::string vertexShaderStr = ReadShader(vertexShaderPath);
	std::string fragmentShaderStr = ReadShader(fragmentShaderPath);
	const char *vertexShader = vertexShaderStr.c_str();
	const char *fragmentShader = fragmentShaderStr.c_str();
	//Create Shaders
	*/
	uint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	uint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(vertexShaderId,1,&vertexShader,NULL);
	glCompileShader(vertexShaderId);
	glCompileShader(fragmentShaderId);
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

std::string readFile(const char *filePath) {
	/*std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;*/
	return "";
}

ProgramManager * ProgramManager::GetInstance()
{
	return nullptr;
}
